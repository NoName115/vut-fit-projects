import numpy as np
import matplotlib.pyplot as plt


input_file = open('meskanie_trat', 'r')
all_stat = []
for line in input_file:
    all_stat.append(line.rstrip('\n').split('\t'))

std = [st[0] for st in all_stat]
x_avg = [round(float(st[1]), 2) for st in all_stat]
x_max = [round(float(st[2]), 2) for st in all_stat]

N = len(all_stat)

ind = np.arange(N)  # the x locations for the groups
width = 0.35       # the width of the bars

fig, ax = plt.subplots()
rects1 = ax.bar(ind, x_avg, width, color='r')
rects2 = ax.bar(ind + width, x_max, width, color='y')

# add some text for labels, title and axes ticks
ax.set_xlabel('Úseky od stanice Žilina')
ax.set_ylabel('Meškanie (s)')
#ax.set_title('Priemerne a maximálne meškanie na úsekoch za Žilinou')
ax.set_xticks(ind + width / 2)
ax.set_xticklabels(std, rotation=50, horizontalalignment='right')
#ax.set_yticks(range(-225, 75, 25))

ax.legend((rects1[0], rects2[0]), ('Priemer', 'Maximum'))

def autolabel(rects):
    """
    Attach a text label above each bar displaying its height
    """
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x() + rect.get_width()/2., 1.05*height,
                '%d' % int(height),
                ha='center', va='bottom')

autolabel(rects1)
autolabel(rects2)

plt.show()
