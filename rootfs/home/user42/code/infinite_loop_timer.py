from time import sleep
from random import randint

counter = 0

while True:
    duration = randint(1, 5)
    sleep(duration)
    counter += duration

print(f"The infinite loop successfully ended after: {counter} seconds")

# Note: this code is not working properly, I should delete it someday and start over.