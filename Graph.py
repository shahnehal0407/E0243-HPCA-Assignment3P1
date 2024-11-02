# import matplotlib.pyplot as plt
# import numpy as np

# # Data for threads and execution times
# threads = [0, 1, 2, 3, 4, 5]
# orig_exec_times = [26.249, 23.962, 24.706, 27.270, 26.209, 26.464]
# mod_exec_times = [5.507, 5.513, 5.506, 5.518, 5.511, 5.510]

# # Set up the figure and axis
# fig, ax = plt.subplots(figsize=(10, 6))

# # Bar width and positioning
# bar_width = 0.4
# x = np.arange(len(threads))

# # Plot vertical bars with vibrant colors
# orig_bars = ax.bar(x - bar_width / 2, orig_exec_times, bar_width, color='#4A90E2', label='Original Execution Time')
# mod_bars = ax.bar(x + bar_width / 2, mod_exec_times, bar_width, color='#50E3C2', label='Modified Execution Time')

# # Add labels, title, and legend
# ax.set_xlabel('Thread Index')
# ax.set_ylabel('Execution Time (s)')
# ax.set_title('Thread Execution Time Comparison (Original vs Modified)')
# ax.set_xticks(x)
# ax.set_xticklabels(threads)
# ax.legend()

# # Add values on top of each bar
# for bar in orig_bars:
#     ax.text(bar.get_x() + bar.get_width() / 2, bar.get_height() + 0.5,
#             f'{bar.get_height():.3f}', ha='center', color='black', fontsize=9)

# for bar in mod_bars:
#     ax.text(bar.get_x() + bar.get_width() / 2, bar.get_height() + 0.5,
#             f'{bar.get_height():.3f}', ha='center', color='black', fontsize=9)

# # Display the plot
# plt.tight_layout()
# plt.show()



import matplotlib.pyplot as plt
import numpy as np

# Data for perf stat metrics
metrics = ['Task Clock (msec)', 'Cycles', 'Instructions', 'Instructions Per Cycle', 
           'L1 Cache Loads', 'L1 Cache Load Misses', 'LLC Loads', 'LLC Load Misses']
before_perf = [154667.46, 476923684707, 313414426100, 0.66, 
               100661285810, 1351052763, 311816841, 5647754]
after_perf = [33156.75, 102222571365, 313275604652, 3.06, 
              100621524252, 27931493, 17890749, 2063900]

# Convert large numbers to logarithmic scale for better visualization
before_perf_log = np.log10(before_perf)
after_perf_log = np.log10(after_perf)

# Plotting the data
fig, ax = plt.subplots(figsize=(12, 6))

# Plot lines for before and after memory alignment
ax.plot(metrics, before_perf_log, label='Before Memory Alignment', marker='o', color='#4A90E2')
ax.plot(metrics, after_perf_log, label='After Memory Alignment', marker='o', color='#50E3C2')

# Add labels and title
ax.set_xlabel('Performance Metrics')
ax.set_ylabel('Logarithmic Value (log10)')
ax.set_title('Comparison of perf stat Metrics (Before vs After Memory Alignment)')
ax.legend()

# Ensure x-axis labels are horizontal
plt.xticks(rotation=0)

# Display the plot
plt.tight_layout()
plt.show()
