fs = 44.1e3;
t = 0:1/fs:1-(1/fs);
x = cos(2*pi*2000*t) + 1/2*sin(2*pi*4000*(t-pi/4));

X = fft(x);
f = (0:length(X)-1)*fs/length(X);

figure(Name="Assignment6")
subplot(3,2,1)
plot(f, abs(X)); 

X_shifted = fftshift(X);
f_shifted = (-length(X)/2:length(X)/2-1)*fs/length(X);

subplot(3,2,2)
plot(f_shifted, abs(X_shifted));

[p,q] = rat(48/44.1);
x_resampled = resample(x, p, q); 
fs_resampled = (p/q) * fs;
L_resampled = length(x_resampled);
t_resampled = (0:L_resampled-1) / fs_resampled; 
f_resampled = (0:L_resampled-1) * (fs_resampled/L_resampled);

subplot(3,2,3)
plot(t_resampled, x_resampled);
xlim([0 0.005]);

subplot(3,2,4)
plot(t, x);
xlim([0 0.005]);

subplot(3,2,5)
plot(f_resampled, abs(fft(x_resampled)));


f_resampled_shifted=(-L_resampled/2:L_resampled/2 - 1)*(fs_resampled/L_resampled);
X_resamp_shifted = fftshift(fft(x_resampled));

subplot(3,2,6)
plot(f_resampled_shifted, abs(X_resamp_shifted));