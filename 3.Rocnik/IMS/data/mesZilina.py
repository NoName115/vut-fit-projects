import numpy as mp
import matplotlib.pyplot as plt


zilina_file = open('meskanie_zilina', 'r')
all_stat = []
for line in zilina_file:
    all_stat.append(line.rstrip('\n').split())

x1 = [int(st[0]) for st in all_stat]
y1 = [round(float(st[1]) * 100, 4) for st in all_stat]

plt.plot(x1, y1, 'o-')
plt.xticks(range(100, 3500, 200))
plt.yticks(range(0, 100, 5))

#plt.title('Žilina - prestup')
plt.xlabel('Meškanie v Žiline (s)')
plt.ylabel('Úspešnosť prestupu v BA (%)')
plt.show()
