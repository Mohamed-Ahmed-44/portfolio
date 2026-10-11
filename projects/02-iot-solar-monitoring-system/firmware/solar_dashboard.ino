#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Firebase_ESP_Client.h>

// Firebase helper macros (required by the library)
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// =====================================================================
// 1. YOUR WIFI CREDENTIALS
// =====================================================================
#define WIFI_SSID     "Staff"
#define WIFI_PASSWORD "Sta@2025"

// =====================================================================
// 1b. FIREBASE CREDENTIALS
// =====================================================================
#define API_KEY       "AIzaSyBjSssQ0a0HxDgI-ur_u337N69H_mM8tjs"
#define USER_EMAIL    "3bsame3@gmail.com"
#define USER_PASSWORD "12345678"
#define DATABASE_URL  "https://ahmed-omar-38911-default-rtdb.europe-west1.firebasedatabase.app/"

// =====================================================================
// 2. HARDWARE PIN DEFINITIONS
// =====================================================================
#define PIN_VOLTAGE     35
#define PIN_CURRENT     33
#define PIN_IRRADIANCE  34
#define PIN_TEMPERATURE 32

// =====================================================================
// 3. CALIBRATION CONSTANTS
// =====================================================================
// INCREASE this number if your dashboard reads lower than your Multimeter
// DECREASE this number if your dashboard reads higher than your Multimeter
#define VOLTAGE_DIVIDER_RATIO   23.0f   
#define ACS712_SENSITIVITY_VA   0.185f  // 0.185 for 5A, 0.100 for 20A, 0.066 for 30A
#define IRRADIANCE_MAX_WM2      1200.0f // Full-scale W/m² for your sensor
#define CALIBRATION_SAMPLES     200     // Samples averaged for zero-current calibration
#define SOLAR_PANEL_AREA_M2     0.0088f // 110mm x 80mm = 0.0088 m²

// =====================================================================
// 4. GLOBAL VARIABLES
// =====================================================================
float g_tempC       = 0.0f;
float g_voltage     = 0.0f;
float g_currentMA   = 0.0f;
float g_powerMW     = 0.0f;
float g_irradiance  = 0.0f;
float g_efficiency  = 0.0f;

// This is measured automatically at boot — no need to hardcode it
int   g_acs712ZeroRaw = 2048;  // will be overwritten during setup calibration

// =====================================================================
// 5. WEB SERVER
// =====================================================================
WebServer server(80);

// =====================================================================
// 5b. FIREBASE OBJECTS
// =====================================================================
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool firebaseReady = false;
unsigned long lastFirebasePush = 0;
unsigned long readingCount = 0;

// =====================================================================
// 6. EMBEDDED DASHBOARD
// =====================================================================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Solar Live Dashboard</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    @import url('https://fonts.googleapis.com/css2?family=Outfit:wght@300;500;700&display=swap');

    :root {
      --bg:     #0b0f19;
      --panel:  rgba(22, 30, 45, 0.75);
      --border: rgba(64, 84, 115, 0.40);
      --cyan:   #00f0ff;
      --pink:   #ff007f;
      --gold:   #ffd700;
      --green:  #00ff88;
      --orange: #ff8c00;
      --text:   #ffffff;
      --muted:  #a0aec0;
    }

    *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

    body {
      background-color: var(--bg);
      background-image:
        radial-gradient(circle at 15% 50%, rgba(0,240,255,.06), transparent 28%),
        radial-gradient(circle at 85% 30%, rgba(255,0,127,.06), transparent 28%);
      color: var(--text);
      font-family: 'Outfit', sans-serif;
      min-height: 100vh;
      padding: 24px 20px 40px;
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 28px;
    }

    header { text-align: center; }
    header h1 {
      font-size: clamp(1.4rem, 4vw, 2.2rem);
      font-weight: 700;
      text-transform: uppercase;
      letter-spacing: 4px;
      text-shadow: 0 0 18px var(--cyan);
    }
    header p {
      color: var(--muted);
      font-size: .85rem;
      margin-top: 4px;
      letter-spacing: 1px;
    }

    .dot {
      display: inline-block;
      width: 8px; height: 8px;
      border-radius: 50%;
      background: var(--green);
      box-shadow: 0 0 8px var(--green);
      margin-right: 6px;
      animation: pulse 1.4s ease-in-out infinite;
    }
    @keyframes pulse { 0%,100%{opacity:1} 50%{opacity:.3} }

    .cards {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(180px, 1fr));
      gap: 16px;
      width: 100%;
      max-width: 1200px;
    }

    .card {
      background: var(--panel);
      border: 1px solid var(--border);
      border-radius: 18px;
      padding: 22px 18px;
      text-align: center;
      backdrop-filter: blur(12px);
      box-shadow: 0 8px 32px rgba(0,0,0,.35);
      transition: transform .25s ease, border-color .25s ease;
    }
    .card:hover { transform: translateY(-5px); border-color: rgba(255,255,255,.18); }
    .card h3 {
      font-size: .75rem;
      font-weight: 300;
      text-transform: uppercase;
      letter-spacing: 2px;
      color: var(--muted);
      margin-bottom: 12px;
    }
    .card .value { font-size: 2.3rem; font-weight: 700; line-height: 1; }
    .card .unit  { font-size: .85rem; font-weight: 300; color: var(--muted); margin-top: 6px; }

    .card:nth-child(1) .value { color: var(--cyan);   text-shadow: 0 0 12px var(--cyan); }
    .card:nth-child(2) .value { color: var(--pink);   text-shadow: 0 0 12px var(--pink); }
    .card:nth-child(3) .value { color: var(--gold);   text-shadow: 0 0 12px var(--gold); }
    .card:nth-child(4) .value { color: var(--green);  text-shadow: 0 0 12px var(--green); }
    .card:nth-child(5) .value { color: var(--orange); text-shadow: 0 0 12px var(--orange); }
    .card:nth-child(6) .value { color: #b52aff;       text-shadow: 0 0 12px #b52aff; }

    .charts {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 20px;
      width: 100%;
      max-width: 1200px;
    }
    @media (max-width: 760px) { .charts { grid-template-columns: 1fr; } }

    .chart-box {
      background: var(--panel);
      border: 1px solid var(--border);
      border-radius: 18px;
      padding: 20px;
      backdrop-filter: blur(12px);
      position: relative;
      height: 380px;
    }

    footer { color: var(--muted); font-size: .75rem; text-align: center; }
  </style>
</head>
<body>

  <header>
    <h1>&#9728;&#65039; Solar Live Dashboard</h1>
    <p><span class="dot"></span>Live &middot; updates every second</p>
  </header>

  <div class="cards">
    <div class="card">
      <h3>Voltage</h3>
      <div class="value"><span id="val_v">--</span></div>
      <div class="unit">Volts (V)</div>
    </div>
    <div class="card">
      <h3>Current</h3>
      <div class="value"><span id="val_c">--</span></div>
      <div class="unit">milliAmps (mA)</div>
    </div>
    <div class="card">
      <h3>Power</h3>
      <div class="value"><span id="val_p">--</span></div>
      <div class="unit">milliWatts (mW)</div>
    </div>
    <div class="card">
      <h3>Temperature</h3>
      <div class="value"><span id="val_t">--</span></div>
      <div class="unit">Celsius (&deg;C)</div>
    </div>
    <div class="card">
      <h3>Irradiance</h3>
      <div class="value"><span id="val_irr">--</span></div>
      <div class="unit">W / m&sup2;</div>
    </div>
    <div class="card">
      <h3>Efficiency</h3>
      <div class="value"><span id="val_eff">--</span></div>
      <div class="unit">Percent (%)</div>
    </div>
  </div>

  <div class="charts">
    <div class="chart-box"><canvas id="ivChart"></canvas></div>
    <div class="chart-box"><canvas id="pvChart"></canvas></div>
  </div>

  <footer>ESP32 &middot; Solar Panel Characterisation &middot; EJUST</footer>

  <script>
    Chart.defaults.color = '#a0aec0';
    Chart.defaults.font.family = "'Outfit', sans-serif";

    const gridColor = 'rgba(64,84,115,0.2)';
    const axisTitle = (text) => ({ display:true, text, color:'#fff', font:{ size:13, weight:'500' }});

    function makeChart(id, label, yLabel, color) {
      return new Chart(document.getElementById(id).getContext('2d'), {
        type: 'scatter',
        data: {
          datasets: [{
            label, data: [],
            backgroundColor: color, borderColor: color,
            pointRadius: 3, pointHoverRadius: 6,
            showLine: true, tension: 0.35, borderWidth: 2
          }]
        },
        options: {
          responsive: true, maintainAspectRatio: false,
          animation: { duration: 0 },
          scales: {
            x: { type:'linear', grid:{ color:gridColor }, title: axisTitle('Voltage (V)') },
            y: { grid:{ color:gridColor }, title: axisTitle(yLabel) }
          },
          plugins: { legend: { labels: { color:'#fff', boxWidth:14 } } }
        }
      });
    }

    const ivChart = makeChart('ivChart', 'I-V Curve', 'Current (mA)', '#00f0ff');
    const pvChart = makeChart('pvChart', 'P-V Curve', 'Power (mW)',   '#ff007f');
    
    // Using Maps to 'bucket' the points. This guarantees exactly ONE dot
    // per Voltage level, completely eradicating vertical zig-zags!
    const ivMap = new Map();
    const pvMap = new Map();

    async function fetchData() {
      try {
        const d = await (await fetch('/data')).json();

        document.getElementById('val_v').innerText   = d.voltage.toFixed(2);
        document.getElementById('val_c').innerText   = d.current.toFixed(1);
        document.getElementById('val_p').innerText   = d.power.toFixed(1);
        document.getElementById('val_t').innerText   = d.temperature.toFixed(1);
        document.getElementById('val_irr').innerText = d.irradiance.toFixed(0);
        document.getElementById('val_eff').innerText = d.efficiency.toFixed(1);

        // Round voltage to nearest 0.02V to cleanly group noisy readings
        const vKey = Math.round(d.voltage * 50) / 50;

        // Apply a smoothing average to the current/power for this specific voltage
        if (!ivMap.has(vKey)) {
          ivMap.set(vKey, d.current);
          pvMap.set(vKey, d.power);
        } else {
          // Smooth out sensor static (80% old value, 20% new value)
          ivMap.set(vKey, ivMap.get(vKey) * 0.8 + d.current * 0.2);
          pvMap.set(vKey, pvMap.get(vKey) * 0.8 + d.power * 0.2);
        }

        // Rebuild chart strictly left-to-right
        ivChart.data.datasets[0].data = Array.from(ivMap, ([x, y]) => ({x, y})).sort((a,b) => a.x - b.x);
        pvChart.data.datasets[0].data = Array.from(pvMap, ([x, y]) => ({x, y})).sort((a,b) => a.x - b.x);

        // Prevent memory overflow
        if (ivMap.size > 200) {
          const oldestKey = ivMap.keys().next().value;
          ivMap.delete(oldestKey);
          pvMap.delete(oldestKey);
        }

        ivChart.update();
        pvChart.update();
      } catch(e) { console.warn('Fetch error:', e); }
    }

    setInterval(fetchData, 1000);
    fetchData();
  </script>
</body>
</html>
)rawliteral";

// =====================================================================
// 7. SERVER HANDLERS
// =====================================================================
void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleData() {
  String json = "{";
  json += "\"voltage\":"     + String(g_voltage,    3) + ",";
  json += "\"current\":"     + String(g_currentMA,  2) + ",";
  json += "\"power\":"       + String(g_powerMW,    2) + ",";
  json += "\"temperature\":" + String(g_tempC,      2) + ",";
  json += "\"irradiance\":"  + String(g_irradiance, 1) + ",";
  json += "\"efficiency\":"  + String(g_efficiency, 2);
  json += "}";
  server.send(200, "application/json", json);
}

// =====================================================================
// 8. SETUP  —  AUTO-CALIBRATES ACS712 ZERO OFFSET AT BOOT
// =====================================================================
void setup() {
  Serial.begin(115200);
  analogReadResolution(12);   // 12-bit → 0–4095

  // ------------------------------------------------------------------
  // AUTO-CALIBRATION: measure the true zero-current ADC midpoint.
  //
  // ⚠️  IMPORTANT: At this moment the ACS712 must have NO current
  //     flowing through it (disconnect the solar panel load if needed).
  //     The ESP32 waits 3 seconds so you have time to do this.
  // ------------------------------------------------------------------
  Serial.println("\n=====================================");
  Serial.println("  ACS712 AUTO-CALIBRATION");
  Serial.println("  Ensure ZERO current through sensor!");
  Serial.println("  Starting in 3 seconds...");
  Serial.println("=====================================");
  delay(3000);

  long sum = 0;
  for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
    sum += analogRead(PIN_CURRENT);
    delay(5);
  }
  g_acs712ZeroRaw = (int)(sum / CALIBRATION_SAMPLES);

  Serial.println("=====================================");
  Serial.print  ("  Calibrated zero offset = ");
  Serial.println(g_acs712ZeroRaw);
  Serial.println("  (theoretical = 2048)");
  Serial.println("=====================================\n");

  // ------------------------------------------------------------------
  // Connect to WiFi
  // ------------------------------------------------------------------
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n=====================================");
  Serial.println("  WiFi Connected!");
  Serial.print  ("  Dashboard → http://");
  Serial.println(WiFi.localIP());
  Serial.println("=====================================\n");

  // ------------------------------------------------------------------
  // Start Web Server
  // ------------------------------------------------------------------
  server.on("/",     handleRoot);
  server.on("/data", handleData);
  server.begin();
  Serial.println("Web server started.");

  // ------------------------------------------------------------------
  // Initialize Firebase
  // ------------------------------------------------------------------
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the callback for token generation (required)
  config.token_status_callback = tokenStatusCallback; // from TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Wait up to 10 seconds for Firebase sign-in
  Serial.print("Signing into Firebase");
  unsigned long fbStart = millis();
  while (!Firebase.ready() && (millis() - fbStart < 10000)) {
    delay(300);
    Serial.print(".");
  }
  if (Firebase.ready()) {
    firebaseReady = true;
    Serial.println("\n=====================================" );
    Serial.println("  Firebase Connected!");
    Serial.println("=====================================\n");
  } else {
    Serial.println("\n  Firebase FAILED — dashboard still works locally.");
  }
}

// =====================================================================
// 9. MAIN LOOP
// =====================================================================
void loop() {
  int raw35 = analogRead(PIN_VOLTAGE);
  int raw33 = analogRead(PIN_CURRENT);
  int raw34 = analogRead(PIN_IRRADIANCE);
  int raw32 = analogRead(PIN_TEMPERATURE);

  // --- Voltage ---
  // Vout = (raw / 4095) * 3.3 V
  // Vpanel = Vout * divider_ratio
  float vOut = (raw35 / 4095.0f) * 3.3f;
  g_voltage  = vOut * VOLTAGE_DIVIDER_RATIO;
  if (g_voltage < 0.0f) g_voltage = 0.0f;

  // --- Current (ACS712) ---
  // Uses the auto-calibrated zero offset measured at boot.
  // fabsf() ensures the reading is always positive (no negative drift).
  float iVout  = (raw33           / 4095.0f) * 3.3f;
  float iVzero = (g_acs712ZeroRaw / 4095.0f) * 3.3f;
  g_currentMA  = fabsf(((iVout - iVzero) / ACS712_SENSITIVITY_VA) * 1000.0f);

  // --- Temperature (LM35) ---
  // LM35: 10 mV per °C, 0 °C = 0 V
  // T(°C) = (raw / 4095) * 3300 mV / 10
  float tVout = (raw32 / 4095.0f) * 3300.0f;
  g_tempC     = tVout / 10.0f;

  // --- Irradiance ---
  // Linearly map 0–4095 ADC counts to 0–IRRADIANCE_MAX_WM2
  g_irradiance = (raw34 / 4095.0f) * IRRADIANCE_MAX_WM2;

  // --- Power (always positive) ---
  g_powerMW = fabsf(g_voltage * g_currentMA);

  // --- Efficiency ---
  // Efficiency (%) = (Power Output / Power Input) * 100
  // Power Output (Watts) = g_powerMW / 1000
  // Power Input  (Watts) = Irradiance (W/m2) * Area (m2)
  float p_watts = g_powerMW / 1000.0f;
  float in_watts = g_irradiance * SOLAR_PANEL_AREA_M2;
  
  if (in_watts > 0.0f) {
    g_efficiency = (p_watts / in_watts) * 100.0f;
  } else {
    g_efficiency = 0.0f;
  }
  // Quietly clamp efficiency mathematically so noise doesn't show 500%
  if (g_efficiency > 100.0f) g_efficiency = 100.0f;

  // Artificial Boost (Disabled): Uncomment the line below if you want the fake +5% boost back!
  // if (g_powerMW > 0.1f) { g_efficiency += 5.0f; }

  // --- Handle web clients ---
  server.handleClient();

  // --- Firebase Push every 1 second ---
  if (firebaseReady && Firebase.ready() && (millis() - lastFirebasePush >= 1000)) {
    lastFirebasePush = millis();
    readingCount++;

    // Build a unique path: /readings/0001, /readings/0002, etc.
    String path = "/readings/" + String(readingCount);

    FirebaseJson json;
    json.set("voltage",     double(g_voltage));
    json.set("current_mA",  double(g_currentMA));
    json.set("power_mW",    double(g_powerMW));
    json.set("temperature", double(g_tempC));
    json.set("irradiance",  double(g_irradiance));
    json.set("efficiency",  double(g_efficiency));
    json.set("timestamp",   double(millis() / 1000.0));

    if (Firebase.RTDB.setJSON(&fbdo, path.c_str(), &json)) {
      Serial.print("[Firebase] Pushed #");
      Serial.println(readingCount);
    } else {
      Serial.print("[Firebase] FAILED: ");
      Serial.println(fbdo.errorReason());
    }
  }

  // --- Serial diagnostics every 1 second ---
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 1000) {
    lastPrint = millis();
    Serial.println("┌─────── SENSOR DIAGNOSTICS ───────");
    Serial.print  ("│ RAW  V35="); Serial.print(raw35);
    Serial.print  ("  I33=");      Serial.print(raw33);
    Serial.print  ("  ZeroI=");    Serial.print(g_acs712ZeroRaw);
    Serial.print  ("  IR34=");     Serial.print(raw34);
    Serial.print  ("  T32=");      Serial.println(raw32);
    Serial.print  ("│ Voltage     : "); Serial.print(g_voltage,   3); Serial.println(" V");
    Serial.print  ("│ Current     : "); Serial.print(g_currentMA, 2); Serial.println(" mA");
    Serial.print  ("│ Power       : "); Serial.print(g_powerMW,   2); Serial.println(" mW");
    Serial.print  ("│ Temperature : "); Serial.print(g_tempC,     1); Serial.println(" °C");
    Serial.print  ("│ Irradiance  : "); Serial.print(g_irradiance,1); Serial.println(" W/m²");
    Serial.print  ("│ Efficiency  : "); Serial.print(g_efficiency,2); Serial.println(" %");
    Serial.print  ("│ Firebase    : "); Serial.println(firebaseReady ? "CONNECTED" : "OFFLINE");
    Serial.print  ("│ IP ADDRESS  : http://"); Serial.println(WiFi.localIP());
    Serial.println("└───────────────────────────────────\n");
  }

  delay(10);
}
