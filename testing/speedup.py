import numc
import dumbpy
import time
import random

W  = '\033[0m'  # white (normal)
R  = '\033[31m' # red
G  = '\033[32m' # green

lst1 = []
for i in range(2500):
  sub = []
  for j in range(2500):
    sub.append(random.uniform(-i, j))
  lst1.append(sub)

lst2 = []
for i in range(2500):
  lst2.append([random.uniform(-i, j)])

lst3 = []
for i in range(1200):
  sub = []
  for j in range(1200):
    sub.append(random.uniform(-i, j))
  lst3.append(sub)

lst4 = []
for i in range(1200):
  lst4.append([random.uniform(-i, j)])

lst5 = []
for i in range(631):
  sub = []
  for j in range(631):
    sub.append(random.uniform(-i, j))
  lst5.append(sub)

lst6 = []
for i in range(631):
  lst6.append([random.uniform(-i, j)])

lst7 = []
for i in range(50):
  sub = []
  for j in range(50):
    sub.append(random.uniform(-i, j))
  lst7.append(sub)

lst8 = []
for i in range(50):
  lst8.append([random.uniform(-i, j)])

print("Lists Made")

fast_mat_large = numc.Matrix(lst1)
fast_vec_large = numc.Matrix(lst2)

fast_mat_med = numc.Matrix(lst3)
fast_vec_med = numc.Matrix(lst4)

fast_mat_weird = numc.Matrix(lst5)
fast_vec_weird = numc.Matrix(lst6)

fast_mat_small = numc.Matrix(lst7)
fast_vec_small = numc.Matrix(lst8)



slow_mat_large = dumbpy.Matrix(lst1)
slow_vec_large = dumbpy.Matrix(lst2)

slow_mat_med = dumbpy.Matrix(lst3)
slow_vec_med = dumbpy.Matrix(lst4)

slow_mat_weird = dumbpy.Matrix(lst5)
slow_vec_weird = dumbpy.Matrix(lst6)

slow_mat_small = dumbpy.Matrix(lst7)
slow_vec_small = dumbpy.Matrix(lst8)

print("Matrices Made")

# Scale
# Add
# Multiply
# Dot
# Outer
# Power
# Tanh/Sigmoid

print("Testing Fast")
start_fast = time.time()

scaled_fast_large = fast_mat_large.scale(3.5)
scaled_fast_vec_large = fast_vec_large.scale(-1.2)
add_fast_large = fast_mat_large.add(fast_mat_large)
mul_fast_large = fast_mat_large.multiply(add_fast_large)
dot_fast_large = scaled_fast_vec_large.dot(fast_vec_large)
outer_fast_large = scaled_fast_vec_large.outer(fast_vec_large)
power_fast_large = fast_mat_large.power(2)
tan_fast_large = fast_mat_large.tanh()

scaled_fast_med = fast_mat_med.scale(3.5)
scaled_fast_vec_med = fast_vec_med.scale(-1.2)
add_fast_med = fast_mat_med.add(fast_mat_med)
mul_fast_med = fast_mat_med.multiply(add_fast_med)
dot_fast_med = scaled_fast_vec_med.dot(fast_vec_med)
outer_fast_med = scaled_fast_vec_med.outer(fast_vec_med)
power_fast_med = fast_mat_med.power(2)
tan_fast_med = fast_mat_med.tanh()

scaled_fast_weird = fast_mat_weird.scale(3.5)
scaled_fast_vec_weird = fast_vec_weird.scale(-1.2)
add_fast_weird = fast_mat_weird.add(fast_mat_weird)
mul_fast_weird = fast_mat_weird.multiply(add_fast_weird)
dot_fast_weird = scaled_fast_vec_weird.dot(fast_vec_weird)
outer_fast_weird = scaled_fast_vec_weird.outer(fast_vec_weird)
power_fast_weird = fast_mat_weird.power(2)
tan_fast_weird = fast_mat_weird.tanh()

scaled_fast_small = fast_mat_small.scale(3.5)
scaled_fast_vec_small = fast_vec_small.scale(-1.2)
add_fast_small = fast_mat_small.add(fast_mat_small)
mul_fast_small = fast_mat_small.multiply(add_fast_small)
dot_fast_small = scaled_fast_vec_small.dot(fast_vec_small)
outer_fast_small = scaled_fast_vec_small.outer(fast_vec_small)
power_fast_small = fast_mat_small.power(2)
tan_fast_small = fast_mat_small.tanh()

end_fast = time.time()
fast_time = end_fast - start_fast
print("Fast took {0}".format(fast_time))

print("Testing Slow")
start_slow = time.time()

scaled_slow_large = slow_mat_large.scale(3.5)
scaled_slow_vec_large = slow_vec_large.scale(-1.2)
add_slow_large = slow_mat_large.add(slow_mat_large)
mul_slow_large = slow_mat_large.multiply(add_slow_large)
dot_slow_large = scaled_slow_vec_large.dot(slow_vec_large)
outer_slow_large = scaled_slow_vec_large.outer(slow_vec_large)
power_slow_large = slow_mat_large.power(2)
tan_slow_large = slow_mat_large.tanh()

scaled_slow_med = slow_mat_med.scale(3.5)
scaled_slow_vec_med = slow_vec_med.scale(-1.2)
add_slow_med = slow_mat_med.add(slow_mat_med)
mul_slow_med = slow_mat_med.multiply(add_slow_med)
dot_slow_med = scaled_slow_vec_med.dot(slow_vec_med)
outer_slow_med = scaled_slow_vec_med.outer(slow_vec_med)
power_slow_med = slow_mat_med.power(2)
tan_slow_med = slow_mat_med.tanh()

scaled_slow_weird = slow_mat_weird.scale(3.5)
scaled_slow_vec_weird = slow_vec_weird.scale(-1.2)
add_slow_weird = slow_mat_weird.add(slow_mat_weird)
mul_slow_weird = slow_mat_weird.multiply(add_slow_weird)
dot_slow_weird = scaled_slow_vec_weird.dot(slow_vec_weird)
outer_slow_weird = scaled_slow_vec_weird.outer(slow_vec_weird)
power_slow_weird = slow_mat_weird.power(2)
tan_slow_weird = slow_mat_weird.tanh()

scaled_slow_small = slow_mat_small.scale(3.5)
scaled_slow_vec_small = slow_vec_small.scale(-1.2)
add_slow_small = slow_mat_small.add(slow_mat_small)
mul_slow_small = slow_mat_small.multiply(add_slow_small)
dot_slow_small = scaled_slow_vec_small.dot(slow_vec_small)
outer_slow_small = scaled_slow_vec_small.outer(slow_vec_small)
power_slow_small = slow_mat_small.power(2)
tan_slow_small = slow_mat_small.tanh()

end_slow = time.time()
slow_time = end_slow - start_slow
print("Slow took {0}".format(slow_time))

print("The speedup is {0} times".format(slow_time/fast_time))

print("=====================================")
print("Now testing that your implementation is correct")
print("=====================================")
print("Checking the small tests")

scaled_fast_small = scaled_fast_small.to_list()
scaled_fast_vec_small = scaled_fast_vec_small.to_list()
add_fast_small = add_fast_small.to_list()
mul_fast_small = mul_fast_small.to_list()
outer_fast_small = outer_fast_small.to_list()
power_fast_small = power_fast_small.to_list()
tan_fast_small = tan_fast_small.to_list()

scaled_slow_small = scaled_slow_small.to_list()
scaled_slow_vec_small = scaled_slow_vec_small.to_list()
add_slow_small = add_slow_small.to_list()
mul_slow_small = mul_slow_small.to_list()
outer_slow_small = outer_slow_small.to_list()
power_slow_small = power_slow_small.to_list()
tan_slow_small = tan_slow_small.to_list()

if (scaled_fast_small != scaled_slow_small):
  print(R+"Small Scale Failed"+W)
else:
  print(G+"Small Scale Passed"+W)


if (scaled_fast_vec_small != scaled_slow_vec_small):
  print(R+"Small Vector Scale Failed"+W)
else:
  print(G+"Small Vector Scale Passed"+W)


if (add_fast_small != add_slow_small):
  print(R+"Small Add Failed"+W)
else:
  print(G+"Small Add Passed"+W)


if (mul_fast_small != mul_slow_small):
  print(R+"Small Multiply Failed"+W)
else:
  print(G+"Small Multiply Passed"+W)


if (dot_fast_small != dot_slow_small):
  print(R+"Small Dot Failed"+W)
else:
  print(G+"Small Dot Passed"+W)


if (outer_fast_small != outer_slow_small):
  print(R+"Small Outer Failed"+W)
else:
  print(G+"Small Outer Passed"+W)


if (power_fast_small != power_slow_small):
  print(R+"Small Power Failed"+W)
else:
  print(G+"Small Power Passed"+W)


if (tan_fast_small != tan_slow_small):
  print(R+"Small Tan Failed"+W)
else:
  print(G+"Small Tan Passed"+W)


print("=====================================")
print("Checking the large tests")

scaled_slow_large = scaled_slow_large.to_list()
scaled_slow_vec_large = scaled_slow_vec_large.to_list()
add_slow_large = add_slow_large.to_list()
mul_slow_large = mul_slow_large.to_list()
outer_slow_large = outer_slow_large.to_list()
power_slow_large = power_slow_large.to_list()
tan_slow_large = tan_slow_large.to_list()

scaled_fast_large = scaled_fast_large.to_list()
scaled_fast_vec_large = scaled_fast_vec_large.to_list()
add_fast_large = add_fast_large.to_list()
mul_fast_large = mul_fast_large.to_list()
outer_fast_large = outer_fast_large.to_list()
power_fast_large = power_fast_large.to_list()
tan_fast_large = tan_fast_large.to_list()

if (scaled_fast_large != scaled_slow_large):
  print(R+"Large Scale Failed"+W)
else:
  print(G+"Large Scale Passed"+W)


if (scaled_fast_vec_large != scaled_slow_vec_large):
  print(R+"Large Vector Scale Failed"+W)
else:
  print(G+"Large Vector Scale Passed"+W)


if (add_fast_large != add_slow_large):
  print(R+"Large Add Failed"+W)
else:
  print(G+"Large Add Passed"+W)


if (mul_fast_large != mul_slow_large):
  print(R+"Large Multiply Failed"+W)
else:
  print(G+"Large Multiply Passed"+W)


if (dot_fast_large != dot_slow_large):
  print(R+"Large Dot Failed"+W)
else:
  print(G+"Large Dot Passed"+W)


if (outer_fast_large != outer_slow_large):
  print(R+"Large Outer Failed"+W)
else:
  print(G+"Large Outer Passed"+W)


if (power_fast_large != power_slow_large):
  print(R+"Large Power Failed"+W)
else:
  print(G+"Large Power Passed"+W)


if (tan_fast_large != tan_slow_large):
  print(R+"Large Tan Failed"+W)
else:
  print(G+"Large Tan Passed"+W)



print("=====================================")
print("Checking the medium tests")

scaled_slow_med = scaled_slow_med.to_list()
scaled_slow_vec_med = scaled_slow_vec_med.to_list()
add_slow_med = add_slow_med.to_list()
mul_slow_med = mul_slow_med.to_list()
outer_slow_med = outer_slow_med.to_list()
power_slow_med = power_slow_med.to_list()
tan_slow_med = tan_slow_med.to_list()

scaled_fast_med = scaled_fast_med.to_list()
scaled_fast_vec_med = scaled_fast_vec_med.to_list()
add_fast_med = add_fast_med.to_list()
mul_fast_med = mul_fast_med.to_list()
outer_fast_med = outer_fast_med.to_list()
power_fast_med = power_fast_med.to_list()
tan_fast_med = tan_fast_med.to_list()

if (scaled_fast_med != scaled_slow_med):
  print(R+"Medium Scale Failed"+W)
else:
  print(G+"Medium Scale Passed"+W)


if (scaled_fast_vec_med != scaled_slow_vec_med):
  print(R+"Medium Vector Scale Failed"+W)
else:
  print(G+"Medium Vector Scale Passed"+W)


if (add_fast_med != add_slow_med):
  print(R+"Medium Add Failed"+W)
else:
  print(G+"Medium Add Passed"+W)


if (mul_fast_med != mul_slow_med):
  print(R+"Medium Multiply Failed"+W)
else:
  print(G+"Medium Multiply Passed"+W)


if (dot_fast_med != dot_slow_med):
  print(R+"Medium Dot Failed"+W)
else:
  print(G+"Medium Dot Passed"+W)


if (outer_fast_med != outer_slow_med):
  print(R+"Medium Outer Failed"+W)
else:
  print(G+"Medium Outer Passed"+W)


if (power_fast_med != power_slow_med):
  print(R+"Medium Power Failed"+W)
else:
  print(G+"Medium Power Passed"+W)


if (tan_fast_med != tan_slow_med):
  print(R+"Medium Tan Failed"+W)
else:
  print(G+"Medium Tan Passed"+W)


print("=====================================")
print("Checking the weird sized tests")

scaled_slow_weird = scaled_slow_weird.to_list()
scaled_slow_vec_weird = scaled_slow_vec_weird.to_list()
add_slow_weird = add_slow_weird.to_list()
mul_slow_weird = mul_slow_weird.to_list()
outer_slow_weird = outer_slow_weird.to_list()
power_slow_weird = power_slow_weird.to_list()
tan_slow_weird = tan_slow_weird.to_list()

scaled_fast_weird = scaled_fast_weird.to_list()
scaled_fast_vec_weird = scaled_fast_vec_weird.to_list()
add_fast_weird = add_fast_weird.to_list()
mul_fast_weird = mul_fast_weird.to_list()
outer_fast_weird = outer_fast_weird.to_list()
power_fast_weird = power_fast_weird.to_list()
tan_fast_weird = tan_fast_weird.to_list()

if (scaled_fast_weird != scaled_slow_weird):
  print(R+"Weird Scale Failed"+W)
else:
  print(G+"Weird Scale Passed"+W)


if (scaled_fast_vec_weird != scaled_slow_vec_weird):
  print(R+"Weird Vector Scale Failed"+W)
else:
  print(G+"Weird Vector Scale Passed"+W)


if (add_fast_weird != add_slow_weird):
  print(R+"Weird Add Failed"+W)
else:
  print(G+"Weird Add Passed"+W)


if (mul_fast_weird != mul_slow_weird):
  print(R+"Weird Multiply Failed"+W)
else:
  print(G+"Weird Multiply Passed"+W)


if (dot_fast_weird != dot_slow_weird):
  print(R+"Weird Dot Failed"+W)
else:
  print(G+"Weird Dot Passed"+W)


if (outer_fast_weird != outer_slow_weird):
  print(R+"Weird Outer Failed"+W)
else:
  print(G+"Weird Outer Passed"+W)


if (power_fast_weird != power_slow_weird):
  print(R+"Weird Power Failed"+W)
else:
  print(G+"Weird Power Passed"+W)


if (tan_fast_weird != tan_slow_weird):
  print(R+"Weird Tan Failed"+W)
else:
  print(G+"Weird Tan Passed"+W)


print("=====================================")
print("Testing finished")


