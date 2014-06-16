description:

    Multimedia Epitivo project
        - takes video from user WebCam and record it to file on HDD, store at catalog;
        - takes files from HDD and plays it

dependencies:

    - For work on that project you need to install QTCreator 3.x and QT Lib 5.2.x
    - any OS is perfect
    - opencv

RGB to YUV:

Given:

Kr = 0.299
Kb = 0.114
L = Kr * R + Kb * B + (1 – Kr – Kb) * G

Then:

Y = round(219 * L / 255) + 16
U = round(224 * 0.5 * (B - L) / ((1 - Kb) * 255)) + 128
V = round(224 * 0.5 * (R - L) / ((1 - Kr) * 255)) + 128

YUV to RGB:

C = Y - 16
D = U - 128
E = V - 128

the formulas to convert YUV to RGB can be derived as follows:

R = clip( round( 1.164383 * C + 1.596027 * E ) )
G = clip( round( 1.164383 * C - (0.391762 * D) - (0.812968 * E) ) )
B = clip( round( 1.164383 * C + 2.017232 * D ) )

where clip() denotes clipping to a range of [0..255].
