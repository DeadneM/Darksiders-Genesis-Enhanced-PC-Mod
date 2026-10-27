# V1.116 - Native effect choices

**451 ParticleSystem exports checked; 43 groups / 98 roles. Runtime EXPERIMENTAL.** New emitted effects only; models/materials remain under their existing controls. Native/default preserves current gameplay and core behavior. Hidden preserves the component lifecycle. Missing loaded assets retain native appearance.

| Category | Menu selector | Choices |
| --- | --- | --- |
| Tools | Caltrops burst | Native / Base / Frost 1 / Frost 2 / Frost 3 / Hidden |
| Tools | Caltrops ground effects | Native / Hidden |
| Tools | Tremor Gauntlet | Native / Level 1 / Level 2 / Level 3 / Hidden |
| Tools | Vorpal Blade | Native / Level 1 / Level 2 / Level 3 / Imbued 1 / Imbued 2 / Imbued 3 / Hidden |
| Tools | Ghost Hook | Native / Hidden |
| Tools | Void Bomb | Native / Hidden |
| Tools | Aether Spark | Native / Spark idle / Hip idle / Hidden |
| Powers | Blade Geyser | Native / Base / Level 1 / Level 2 / Level 3 / Hidden |
| Powers | World Ender beam | Native / Base / Frost / Hidden |
| Powers | World Ender death blast | Native / Level 1 / Level 2 / Level 3 / Hidden |
| Powers | Rampage | Native / Base / Chaos / Hidden |
| Powers | Stoneskin particles | Native / Base / Dust and coins / Base without dust / Molten / Molten without dust / Hidden |
| Powers | Shadow body aura | Native / Effect 1 / Effect 2 / Hidden |
| Powers | Synergy clones | Native / Effect 1 / Effect 2 / Effect 3 / Effect 4 / Hidden |
| Strife ammunition | Beam Shot | Native / Normal / Hotstreak / Hidden |
| Strife ammunition | Beam Shot impact | Native / Normal / Hotstreak 1 / Hotstreak 2 / Hidden |
| Strife ammunition | Charge Shot | Native / Level 1 / Level 2 / Level 3 / Hotstreak / Hidden |
| Strife ammunition | Graviton Shot | Native / Level 1 / Level 2 / Level 3 / Hotstreak / Hidden |
| Strife ammunition | Lava muzzle flash | Native / Normal / Hotstreak / Hidden |
| Strife ammunition | Lava other effects | Native / Hidden |
| Strife ammunition | Nature Shot | Native / Normal / Hotstreak / Hidden |
| Strife ammunition | Static Shot | Native / Normal / Hotstreak / Hidden |
| Strife ammunition | Standard shot impact | Native / Normal / Hotstreak / Hidden |
| War weapons | Earthsplitter | Native / Level 1 / Level 2 / Level 3 / Hidden |
| War weapons | Flamebrand | Native / Level 1 / Level 2 / Level 3 / Hidden |
| War weapons | Grasping Shadows | Native / Base / Level 1 / Level 2 / Hidden |
| War weapons | Thunderclap / core lightning | Native / Level 1 / Level 2 / Level 3 / Hidden |
| War weapons | Vampiric | Native / Light / Level 1 / Level 2 / Level 3 / Hidden |
| War weapons | Vortex finisher | Native / Level 1 / Level 2 / Level 3 / Hidden |
| Mounts and forms | Mount effects | Native / Ruin / Rampage / Despair / Mayhem / Hidden |
| Mounts and forms | Ultimate effects | Native / Chaos / Havoc / Hidden |
| Creature cores | Molten Fallen aura | Native / Level 1 / Level 2 / Level 3 / Hidden |
| Creature cores | Jailer aura | Native / Level 1 / Level 2 / Level 3 / Hidden |
| Creature cores | Tidehunter blast | Native / Core level 1 / Core level 2 / Core level 3 / Hidden |
| Creature cores | Wicked K blast | Native / Level 1 / Level 2 / Level 3 / Hidden |
| Creature cores | Nephilim blast | Native / Level 1 / Level 2 / Level 3 / Hidden |
| Creature cores | Astarte | Native / Hidden |
| Creature cores | Scarab acid | Native / Hidden |
| Creature cores | Shield Demon | Native / Hidden |
| Creature cores | Legion | Native / Hidden |
| Other effects | Other Strife effects | Native / Hidden |
| Other effects | Other War effects | Native / Hidden |
| Other effects | Other mount effects | Native / Hidden |

World Ender scale roles remain paired. Tidehunter core 2 really references Level3, and core 3 Level2; the table preserves that native ordering. Presets with no native counterpart for a role retain that role rather than inventing an asset. Existing SCS components may not pass through the creation hook.

Body Stoneskin, Leviathan, Stinger, armors, War arm and pistol/model controls are retained separately. Source family data is in [the exact checked table](evidence/V1_116_EFFECT_LIBRARY.json).
