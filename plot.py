"""@package docstring
File: This module is used to visualize the results of the output vector
author: Xufei Gao
"""


import graphviz
import matplotlib.pyplot as plt
from PIL import Image


# This function aims at finding the number of lines in the given file
# and return the number
def number_of_lines(file):
    """
    Given a .txt file name, this function aims at finding
    the number of lines in the given file, and return the
    number

    Parameters
    ----------
    file : a .txt file name in string format
    """
    store = open(file, 'r')
    line = store.readline()
    line_number = 0
    while line:
        line_number += 1
        line = store.readline()
    store.close()
    return line_number


# This function aims at slicing data in the target .txt document
# and calling process_line function to process the data to generate diagrams
# return the number of lines in the given file
def plot_figure(file):
    """
    Given a .txt file name, this function aims at slicing data in the
    target .txt document by line, and calling process_line function to process
    the data to generate initial diagrams. At last, return the number of
    lines in the given file

    Parameters
    ----------
    file : a .txt file name in string format
    """
    each_line = []
    slices = [0, 1, 3, 5, 8, 9, 11, 26, 50, 60, 70, 80, 90, 100, 110,
              150, 160, 170, 180, 200, 220, 250, 270, 300, 350, 400, 500, 2999]
    store = open(file, 'r')
    count = 0
    # max_line = number_of_lines(file)
    for lines in store:
        if count in slices:
            ls = lines
            slices.append(count)
            for i in ls.split(" "):
                each_line.append(i)
            process_line(each_line, count)
        each_line = []
        count += 1
    return slices


# The function takes graphviz to process the data from the plot_figure function
# and generate corresponding diagrams
def process_line(line, count):
    """
    Accept the data in a .txt document line by line from plot_figure function,
    and a integer called count, this integer is use to name the each generated
    diagram. As the each time the plot_figure function transfers the count to
    this function, the value of count is different, which is bigger than before
    by one. So the generated diagrams could be seperated by the count.

    Parameters
    ----------
    line : the data in a line of a .txt document
    count: a integer which represents the line number
    """

    # make a contain to store the processed data
    transfer = []
    # record the fitness value of each step
    value = line[-1]
    del line[-1]
    length = len(line)
    # process data
    for i in range(length):
        if int(line[i]) == length // 2:
            transfer.append('Concentrate')
        elif int(line[i]) == length // 2 + 1:
            transfer.append('Tailings')
        else:
            transfer.append('Unit '+str(line[i]))
    print(transfer)
    # start with a graphviz object
    graph = graphviz.Digraph()
    # Graph layout direction from left to right
    graph.attr(rankdir='LR')
    # Set the shape of node into rectangle
    graph.attr('node', shape='rectangle')
    # this sets x and y margins of canvas, in inches
    graph.attr(margin="0,0")
    # add label which show the fitness value of each step
    graph.attr(label='fitness value = '+str(value), fontsize='40')
    # set the relationship between nodes
    graph.edge('Feed', 'Unit '+str(line[0]), color='blue', headport='w',
               tailport='e', arrowhead='normal',
               arrowtail='normal')

    for i in range(length // 2):
        graph.edge('Unit '+str(i), transfer[i*2+1], color='blue',
                   headport='w', tailport='n',
                   arrowhead='normal')
        graph.edge('Unit '+str(i), transfer[i*2+2], color='red',
                   headport='w', tailport='s',
                   arrowhead='normal')
    # generate diagram into png format and store it into the given place
    graph.render(filename='./store_figures/figure'+'_'+str(count),
                 cleanup=True, format='png')


slices = plot_figure("./src/data.txt")

# process the diagrams generated before into fixed size
# and flag the order of each diagram
for i in range(len(slices)):
    img = plt.imread('./store_figures/figure'+'_'+str(slices[i])+'.png')
    fig, ax = plt.subplots(1, 1, figsize=(18, 9))
    ax.set_title('No.'+str(slices[i])+' Step', fontsize=30, y=1.03)
    plt.imshow(img)
    plt.axis('off')
    ax.axis('off')
    plt.savefig('./processed_figures/figure'+'_'+str(slices[i])+'.png')
    plt.close()


# collect all the processed diagrams and use them to generate a .gif document
# show the dynamic graph of the convergence path towards the optimal circuit
image = []
for i in range(len(slices)):
    new = Image.open('./processed_figures/figure'+'_'+str(slices[i])+'.png')
    image.append(new)
image[0].save('SST.gif', format='GIF', append_images=image[1:],
              save_all=True, duration=500, loop=0)
print('gif finished')
