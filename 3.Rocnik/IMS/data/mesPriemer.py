import numpy as np
import matplotlib.pyplot as plt


input_file = open('meskanie_priemer', 'r')
all_stat = []
for line in input_file:
    all_stat.append(line.rstrip('\n').split('\t'))

x1 = [st[1] for st in all_stat]
y1 = [round(float(st[2]), 4) for st in all_stat]

ind = np.arange(len(all_stat))
width = 0.4

fig, ax = plt.subplots()
rects1 = ax.bar(ind, y1, width, color='r')

# add some text for labels, title and axes ticks
ax.set_ylabel('Zmena meškania (%)')
ax.set_xlabel('Stanice linky Košice-Bratislava')
#ax.set_title('Priemerna zmena meškania podľa stanice')
ax.set_xticks(ind + width / 2)
ax.set_xticklabels(x1, rotation=50, horizontalalignment='right')
ax.set_yticks(range(-20, 25, 5))

plt.show()
