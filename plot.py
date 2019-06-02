import numpy as np
from scipy.stats import linregress
import json
import sys
import getopt
try:
    from matplotlib import pyplot as plt
except Exception:
    import matplotlib
    matplotlib.use('pdf')
    from matplotlib import pyplot as plt


def save_plot(alg_name, file_suffix, y_label, legend_list, title_prefix,
              is_log=False, axes=None, x_label='vertices', const_ve=0):
    plt.title('{0} for {1} for {2} {3}'.format(
        title_prefix, alg_name, const_ve,
        'edges' if x_label is 'vertices' else 'vertices'
        )
    )

    plt.legend(legend_list, loc='upper left')
    if is_log:
        plt.xlabel('Log of no. of {0}'.format(x_label))
        axes.get_legend().remove()
    else:
        plt.xlabel('No. of {0}'.format(x_label))
    plt.ylabel(y_label)
    plt.grid()
    plt.ticklabel_format(axis='both',
                         style='sci',
                         scilimits=(-3, 3),
                         useOffset=False)
    plt.savefig('out/pdf/' + alg_name + '_' + file_suffix + '.pdf')
    plt.clf()


def plot_mst():
    mst_data = []
    with open('out/mst.jsonl', 'r') as mst_file:
        for line in mst_file:
            mst_data.append(json.loads(line))
    verts = [item['verts'] for item in mst_data]
    edges = [item['edges'] for item in mst_data]
    times_dicts = [item['times'] for item in mst_data]
    times = {}
    times['Boruvka'] = [time['Boruvka'] for time in times_dicts]
    times['Kruskal'] = [time['Kruskal'] for time in times_dicts]

    if all(vert == verts[0] for vert in verts):
        x_label = 'edges'
        const_ve = verts[0]
        plt.plot(edges, times['Boruvka'])
        plt.plot(edges, times['Kruskal'])
    else:
        x_label = 'vertices'
        const_ve = edges[0]
        plt.plot(verts, times['Boruvka'])
        plt.plot(verts, times['Kruskal'])

    legend = [
        'Boruvka',
        'Kruskal'
    ]
    save_plot('mst', x_label, 'Time in microseconds',
              legend,
              'Exec time',
              x_label=x_label, const_ve=const_ve)

    linreg_text = ''
    ax = plt.axes()
    for alg in legend[:5]:
        slope, err = plot_log(times[alg],
                              verts if x_label is 'vertices' else edges)
        linreg_text += '{0}: slope={1}, err={2}\n'.format(
            alg, np.around(slope, 3), np.around(err, 3)
        )
    plt.text(0.25, 0.05,  # position of the text relative to axes
             '  Linregress:\n{0}'.format(linreg_text),
             horizontalalignment='left',
             verticalalignment='baseline',
             transform=ax.transAxes,
             fontdict=dict(
                           family='monospace',
                           color='darkred',
                           weight='bold',
                           size=12)
             )
    save_plot('mst',
              'log_log_{0}'.format(x_label),
              'Log of exec time',
              [''],
              'Log of exec time',
              is_log=True,
              axes=ax,
              x_label=x_label,
              const_ve=const_ve)


def plot_minpath():
    minpath_data = []
    plot_dfs = True
    with open('out/minpath.jsonl', 'r') as mst_file:
        for line in mst_file:
            minpath_data.append(json.loads(line))
    verts = [item[0] for item in minpath_data]
    edges = [item[1] for item in minpath_data]
    times = {}
    times['dij_mat_tab'] = [item[2] for item in minpath_data]
    times['dij_mat_heap'] = [item[3] for item in minpath_data]
    times['dij_tab_tab'] = [item[4] for item in minpath_data]
    times['dij_tab_heap'] = [item[5] for item in minpath_data]
    times['floyd'] = [item[6] for item in minpath_data]
    try:
        times['dfs'] = [item[7] for item in minpath_data]
    except Exception:
        times['dfs'] = []
        plot_dfs = False

    if all(vert == verts[0] for vert in verts):
        # If all verts are the same, make plot for edge number
        x_label = 'edges'
        const_ve = verts[0]
        plt.plot(edges, times['dij_mat_tab'])
        plt.plot(edges, times['dij_mat_heap'])
        plt.plot(edges, times['dij_tab_tab'])
        plt.plot(edges, times['dij_tab_heap'])
        plt.plot(edges, times['floyd'])
        if plot_dfs:
            plt.plot(edges, times['dfs'])
    else:
        x_label = 'vertices'
        const_ve = edges[0]
        plt.plot(verts, times['dij_mat_tab'])
        plt.plot(verts, times['dij_mat_heap'])
        plt.plot(verts, times['dij_tab_tab'])
        plt.plot(verts, times['dij_tab_heap'])
        plt.plot(verts, times['floyd'])
        if plot_dfs:
            plt.plot(verts, times['dfs'])

    legend = [
        'dij_mat_tab',
        'dij_mat_heap',
        'dij_tab_tab',
        'dij_tab_heap',
        'floyd',
        'dfs'
    ]
    save_plot('minpath', x_label, 'Time in microseconds',
              legend if plot_dfs else legend[:5],
              'Exec time',
              x_label=x_label, const_ve=const_ve)

    linreg_text = ''
    ax = plt.axes()
    for alg in legend[:5]:
        slope, err = plot_log(times[alg],
                              verts if x_label is 'vertices' else edges)
        linreg_text += '{0}: slope={1}, err={2}\n'.format(
            alg, np.around(slope, 3), np.around(err, 3)
        )
    plt.text(0.25, 0.05,  # position of the text relative to axes
             '  Linregress:\n{0}'.format(linreg_text),
             horizontalalignment='left',
             verticalalignment='baseline',
             transform=ax.transAxes,
             fontdict=dict(
                           family='monospace',
                           color='darkred',
                           weight='bold',
                           size=12)
             )
    save_plot('minpath',
              'log_log_{0}'.format(x_label),
              'Log of exec time',
              [''],
              'Log of exec time',
              is_log=True,
              axes=ax,
              x_label=x_label,
              const_ve=const_ve)


def plot_log(execution_time_array, ve_number_arr):
    '''Log plot of exec time
    Not very universal, you may have to tweak
    some numbers'''
    data_big_val = ve_number_arr

    if 0 not in execution_time_array:
        exec_time_log_arr = np.log2(execution_time_array)
        data_big_val_log = np.log2(data_big_val)
    else:
        print('Some of the values in exec_time are 0')
        print('and logarithm of 0 is minus infinity.')
        print('Discarding those values for this plot')
        exec_time_arr = [x for x in execution_time_array if x is not 0]
        exec_time_log_arr = np.log2(exec_time_arr)
        arr_start = len(data_big_val) - len(exec_time_arr)
        data_big_val_log = np.log2(data_big_val[arr_start:])

    slope, _, _, _, err = linregress(data_big_val_log, exec_time_log_arr)
    plt.plot(
        data_big_val_log, exec_time_log_arr
    )
    return slope, err


def main(argv):
    try:
        opts, _ = getopt.getopt(argv, 'mp')
    except getopt.GetoptError:
        print('error')
        exit()
    for opt, _ in opts:
        if opt == '-m':
            plot_mst()
        if opt == '-p':
            plot_minpath()


if __name__ == '__main__':
    main(sys.argv[1:])
