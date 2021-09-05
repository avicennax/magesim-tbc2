import gym
from gym import error, spaces, utils
from gym.utils import seeding

from magesim.simulation import Sim


class MageSimEnv(gym.Env):
    metadata = {'render.modes': ['human']}

    def __init__(self):
        self.sim = Sim()

    def step(self, action):
        pass

    def run(self):
        self.sim.run()
        self.sim.print_log()

    def reset(self):
        pass

    def render(self, mode='human'):
        pass

    def close(self):
        pass