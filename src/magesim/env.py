"""OpenAI Gym Wrapper around TBC MageSim"""
from collections import namedtuple
import sys
from typing import Any, Dict, Tuple

import gym
from gym import spaces
from gym.utils import seeding
import numpy as np

from magesim.simulation import Sim


class MageSimEnv(gym.Env):
    """TBC MageSim Wrapper Env

    Currently only supports offensive spell actions; supported spells below:
        ARCANE_BLAST = 30451
        ARCANE_MISSILES = 38699
        FROSTBOLT = 27072
        FIREBALL = 27070
        SCORCH = 27074
        PYROBLAST = 33938

    Current observation space struct has follow fields, more details see env.h:
        double t;
        double gcd;
        double mana;
        double duration;
        Spec spec;
        int dmg;
        int mana_emerald;
        int mana_ruby
    """
    StateDict = Dict[str, Any]
    StepResponse = namedtuple(
        "StepReponse", ["observation", "reward", "done", "info"]
    )

    metadata = {'render.modes': ['human']}
    ACTION_SPACE_SIZE = 6
    SPEC_NAME = "magesim"
    VERSION = "0"

    _ACTION_SAMPLE_TO_ID_MAP = {
        i: action_id for i, action_id in enumerate(
            [30451, 38699, 27072, 27070, 27074, 33938]
        )
    }

    action_space = spaces.Discrete(ACTION_SPACE_SIZE)

    # Consider using space.Dict -- need to determine
    # if Spinning Up algorithms will play nice.
    observation_space = spaces.Tuple(
        [spaces.Box(-np.inf, np.inf, shape=(), dtype=np.float32) for _ in range(4)] + \
        [spaces.Box(-sys.maxsize, sys.maxsize, shape=(), dtype=np.int64) for _ in range(4)]
    )
    # Tuple space needs have size manually set.
    observation_space.shape = (len(observation_space),)

    def __init__(self):
        # Initialize underlying simulation
        self.sim = Sim()
        self.sim.bootstrap()

        # Track certain state here to avoid refactoring Sim.
        self.dmg = 0
        self.t = 0

    def step(self, action: int):
        # Call int incase action is a 1D ndarray.
        action_id = self._ACTION_SAMPLE_TO_ID_MAP[int(action)]
        
        state = self.sim.step(action_id)
        r = self._get_step_response(state)
        self._update_env_instance_state(state)

        return tuple(r)

    def reset(self) -> Tuple:
        self.dmg = 0
        self.t = 0
        return self._get_step_response(self.sim.reset()).observation

    def render(self, mode='human'):
        self.sim.print_log()

    def close(self):
        del self.sim

    def seed(self, seed=None):
        _, seed = seeding.np_random(seed)
        self.sim.seed(seed)
        return [seed]

    def _terminal(self, state_dict: StateDict) -> bool:
        return state_dict["t"] >= state_dict["duration"]

    def _update_env_instance_state(self, state_dict: StateDict):
        self.t = state_dict["t"]
        self.dmg = state_dict["dmg"]

    def _get_reward(self, state_dict: StateDict) -> int:
        return state_dict["dmg"] - self.dmg

    def _get_step_response(self, state_dict: StateDict) -> StepResponse:
        reward = self._get_reward(state_dict)
        done = self._terminal(state_dict)
        observation = tuple(state_dict.values())

        return self.StepResponse(observation, reward, done, {"duration": state_dict["duration"]})