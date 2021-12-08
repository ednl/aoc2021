# Linear, minimum for x = 350
def f(x):
    return abs(x - 350) + 100

# Quadratic, minimum for x = 480
def g(x):
    return x * x - 960 * x + 230600

# Assume function has one minimum between a and b
def bisect(a, b, fun):
    loop = 0
    res = 0
    while (a < b):
        loop += 1
        c = (a + b) // 2
        res = fun(c)
        dc = fun(c + 1) - res
        if dc > 0:     # rising
            b = c      # new upper bound
        elif dc < 0:   # falling
            a = c + 1  # new lower bound
        else:          # constant
            break      # found it
    return res, loop

# Min/max from data
a = 0
b = 1878

print("Part 1:", bisect(a, b, f))
print("Part 2:", bisect(a, b, g))
