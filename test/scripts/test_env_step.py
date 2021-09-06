from magesim import env
import gym

spell = 2 # 27072 # FB
steps = 2
render = True

e = gym.make('magesim-v0')
print(e.seed(42))

done = False
for n in range(steps):
# while not done:
    print("\n-- New Step")
    state = e.step(spell)
    print(state)
    done = state[2]

if render:
    e.render()

e.close()