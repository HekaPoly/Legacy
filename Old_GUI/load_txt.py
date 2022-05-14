import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

data = np.load("freestyle_30s_2000Hz_3.npy")
fig = plt.figure()

subplots = []

x_arr = np.linspace(0, float(len(data[0]))/2000.0, len(data[0]))
subplots.append(plt.subplot(241))
subplots.append(plt.subplot(242))
subplots.append(plt.subplot(243))
subplots.append(plt.subplot(244))
subplots.append(plt.subplot(245))
subplots.append(plt.subplot(246))
subplots.append(plt.subplot(247))


def animate(i):
    for index, subplot in enumerate(subplots):
        y_arr = data[index]
        subplot.clear()
        subplot.set_ylim(400, 600)
        subplot.set_xlim(i*0.1-5, i*0.1)
        subplot.plot(x_arr, y_arr)


ani = animation.FuncAnimation(fig, animate, interval=100)
plt.show()
