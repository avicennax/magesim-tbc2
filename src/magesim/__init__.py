from gym.envs.registration import register, registry
from gym.error import UnregisteredEnv

from .env import MageSimEnv

try:
    registry.spec(f"{MageSimEnv.SPEC_NAME}-v{MageSimEnv.VERSION}")
except UnregisteredEnv:
    register(
        id='magesim-v0',
        entry_point='magesim.env:MageSimEnv',
    )