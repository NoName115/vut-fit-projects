1
[x, Fs] = audioread('xkolcu00.wav');
Fs;		% vzorkovacia frekvencia
N = length(x);	% dlzka signalu
t = length(x)/Fs;	% dlzka v sekundach

2
% zobrazenie modulu spektra signalu
X = abs(fft(x));		% fourir. trans. povodneho signalu
om_f = ((0 : (N / 2) - 1) / N * Fs);	% získame frekvencnu osu
X = X(1 : N / 2);	% korekcia poctu vzorkou
figure(1);
plot(om_f, X);
title('Spektrum vstupného signálu xkolcu00');
xlabel('Hz');

3
pos = (find(X == max(X), 1, 'first') - 1) * Fs / length(X) / 2;	% maximum spektra

4
b = [0.2324, -0.4112, 0.2324];
a = [1.0000, 0.2289, 0.4662];
filter(b, a, x);
ukazmito(b, a, Fs);

5
H = freqz(b, a, 256);
f = (0:255) / 256 * Fs / 2;
figure(1);
plot(f, abs(H));
xlabel('f');
ylabel('|H(f)|');

6
fx = filter(b, a, x);
fabs = abs(fft(fx));
om_f = ((0 : (N / 2) - 1) / N * Fs);
fabs = fabs(1:N / 2);
figure(1);
plot(om_f, fabs); xlabel('Hz');

7
fpos = (find(fabs == max(fabs), 1, 'first')-1) * Fs / length(fabs) / 2

9 + 10
function exercise9(x, N)
    R = zeros(1,100);
    R;
    for k = -50:50,
        vys = 0;
        for n = 1:N,
            if n + k > 0 && n + k < N
                vys = vys + x(n) * x(n + k);
            end
        end
        R(k + 51) = 1 / n * vys;
        R(k + 51);
    end
    figure(1);
    stem(-50:50, R); xlabel('k'); ylabel('R[k]');
    R(10)
end
exercise9(x, length(x));