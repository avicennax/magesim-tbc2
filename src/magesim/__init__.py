from gym.envs.registration import register

register(
    id='magesim-v0',
    entry_point='magesim.env:MageSimEnv',
)