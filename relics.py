from enum import IntEnum
from typing import List
import matplotlib.pyplot as plt
import numpy as np
class Substat(IntEnum):
  HP = 0
  HP_P = 1
  ATK = 2
  ATK_P = 3
  DEF = 4
  DEF_P = 5
  CR = 6
  CD = 7
  EF_RES = 8
  EHR = 9
  SPD = 10
  BREAK = 11

substat_prob = np.fromiter([0.1,0.1,0.1,0.1,0.1,0.1,0.06,0.06,0.08,0.08,0.04,0.08], dtype=float)

class Relic():
  def __init__(self, stats):
    self.stats = [False] * 12
    for i in range(4):
      self.stats[stats[i]] = True
    self.double_crit = self.stats[Substat.CD] and self.stats[Substat.CR]
    self.single_crit = not self.double_crit and (self.stats[Substat.CD] or self.stats[Substat.CR])


def simulate(n):
  relics = []
  for i in range(n):
    relics.append(Relic(np.random.choice(12, 4, False, substat_prob)))
  
  return relics

def analize(relics: List[Relic]):
  single_crit = 0
  single_crit_speed = 0
  single_crit_speed_atk = 0
  single_crit_speed_ehr = 0
  single_crit_speed_atk_ehr = 0
  double_crit = 0
  double_crit_speed = 0
  double_crit_speed_atk = 0
  double_crit_speed_ehr = 0
  double_crit_speed_ehr = 0
  for relic in relics:
    if (relic.single_crit):
      single_crit += 1
      if (relic.stats[Substat.SPD]):
        single_crit_speed += 1
        if (relic.stats[Substat.EHR]):
          single_crit_speed_ehr += 1
          if (relic.stats[Substat.ATK_P]):
            single_crit_speed_atk_ehr += 1
        if (relic.stats[Substat.ATK_P]):
          single_crit_speed_atk += 1

    if (relic.double_crit):
      double_crit += 1
      if (relic.stats[Substat.SPD]):
        double_crit_speed += 1
        if (relic.stats[Substat.EHR]):
          double_crit_speed_ehr += 1
        if (relic.stats[Substat.ATK_P]):
          double_crit_speed_atk += 1
  
  x = ["s_crit", "s_crit speed", "s_crit speed atk", "s_crit speed ehr", "s_crit speed atk ehr", "d_crit", "d_crit speed", "d_crit speed atk", "d_crit speed ehr"]
  y = [single_crit, single_crit_speed, single_crit_speed_atk, single_crit_speed_ehr, single_crit_speed_atk_ehr, double_crit, double_crit_speed, double_crit_speed_atk, double_crit_speed_ehr]
  bars = plt.bar(x, y)
  plt.xticks(rotation=30, ha='right')
  plt.yticks
  plt.bar_label(bars, y)

  plt.show()

relics = simulate(1_000_000_000)
analize(relics)

