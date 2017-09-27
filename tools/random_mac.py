from random import randint

def generate_random_mac():
    parts = []
    for i in range(0, 6):
        parts.append(format(randint(16, 255), 'x'))
    return ':'.join(parts)

print(generate_random_mac())
