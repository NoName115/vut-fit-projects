import numpy as mp
import matplotlib.pyplot as plt


def show_plot(input_delay, figure_indx):
    input_file = open('meskanie_trat', 'r')
    all_stat = []
    for line in input_file:
        all_stat.append(line.rstrip('\n').split())

    std = ['Zi'] + [st[0] for st in all_stat]
    avg_list = [round(float(st[1]), 2) for st in all_stat]
    max_list = [round(float(st[2]), 2) for st in all_stat]

    y1 = [input_delay]
    y2 = [input_delay]
    for avg_delay, max_delay in zip(avg_list, max_list):
        y1_delay = round(y1[-1] + avg_delay, 2)
        y1.append(y1_delay if (y1_delay > 0) else 0)
        y2_delay = round(y2[-1] + max_delay, 2)
        y2.append(y2_delay if (y2_delay > 0) else 0)

    print(input_delay)

    fig = plt.figure(figure_indx)
    fig.canvas.set_window_title('Figure_4-' + str(figure_indx))

    line_avg, = plt.plot(range(len(std)), y1, 'o-', label='Priemer')
    line_max, = plt.plot(range(len(std)), y2, 'o-', label='Maximum')
    plt.legend(handles=[line_avg, line_max])

    plt.xticks(range(len(std)), std, rotation=50, horizontalalignment='right')#, size='large')
    plt.yticks(range(0, int(input_delay), int(input_delay / 13)))

    plt.xlabel('Úseky od stanice Žilina')
    plt.ylabel('Meškanie (s)')


delays = [300.0, 1000.0, 1400.0]
for indx, delay in enumerate(delays):
    show_plot(delay, indx)

plt.show()
