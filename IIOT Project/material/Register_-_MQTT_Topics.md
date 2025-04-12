# Register - MQTT Topics
## List
| address | type | MQTT Topics | description | code | Relation |
|:-------:|:----:|:--------------------------:|:-----------------------------------------:| :-----: | :--------: |
| 3012H~3013H | float | IoT/current/I1 | Current I1 | F1 | I=Rxx(CT1/CT2) |
| 3014H~3015H | float | IoT/current/I2 | Current I2 | F1 | I=Rxx(CT1/CT2) | 
| 3016H~3017H | float | IoT/current/I3 | Current I3 | F1 | I=Rxx(CT1/CT2) |
| 3018H~3019H | float | IoT/current/avgcurrent | Average current Iavg | F1 | I=Rxx(CT1/CT2) |
| 4000H~4001H | float | IoT/frequency/frequency | Frequency | F1 | F=Rx |
| 4002H~4003H | float | IoT/phasevoltage/V1 | Phase voltage V1 | F1 | U=Rx*(PT1/PT2) |
| 4004H~4005H | float | IoT/phasevoltage/V2 | Phase voltage V2 | F1 | U=Rx*(PT1/PT2) |
| 4006H~4007H | float | IoT/phasevoltage/V3 | Phase voltage v3 | F1 | U=Rx*(PT1/PT2) |
| 4008H~4009H | float | IoT/phasevoltage/avgvoltage | Average voltage Vavg | F1 | U=Rx*(PT1/PT2) |
| 400aH~400bH | float | IoT/linevoltage/V12 | Line voltage V12 | F1 | U=Rx*(PT1/PT2) |
| 400cH~400dH | float | IoT/linevoltage/V23 | Line voltage V23 | F1 | U=Rx*(PT1/PT2) |
| 400eH~400hH | float | IoT/linevoltage/V31 | Line voltage V31 | F1 | U=Rx*(PT1/PT2) |
| 4010H~4011H | float | IoT/linevoltage/avglinevoltage | Average line voltage | F1 | U=Rx*(PT1/PT2) |
| 4012H~4013H | float | IoT/phasecurrent/I1 | Phase line current I1 | F1 | I=Rxx(CT1/CT2) |
| 4014H~4015H | float | IoT/phasecurrent/I2 | Phase line current I2 | F1 | I=Rxx(CT1/CT2) |
| 4016H~4017H | float | IoT/phasecurrent/I3 | Phase line current I3 | F1 | I=Rxx(CT1/CT2) |
| 4012H~4013H | float | IoT/phasecurrent/avgcurrent | Average line current | F1 | I=Rxx(CT1/CT2) |
| 401aH~401bH | float | IoT/neutrelcurrent/neutral | Neutrel current In | F1 | I=Rxx(CT1/CT2) |
| 401cH~401dH | float | IoT/phasepower/A | Phase A power Pa | F1 | P=Rx*(PT1/PT2)*(CT1/CT2) |
| 401eH~401fH | float | IoT/phasepower/B | Phase B power Pb | F1 | P=Rx*(PT1/PT2)*(CT1/CT2) |
| 4020H~4021H | float | IoT/phasepower/C | Phase C power Pc | F1 | P=Rx*(PT1/PT2)*(CT1/CT2) |
| 4022H~4023H | float | IoT/powerp/powerpsum | System power Psum | F1 | P=Rx*(PT1/PT2)*(CT1/CT2) |
| 4042H~4043H | float | IoT/powerp/powerdemand | Power demand | F1 | P=Rx*(PT1/PT2)*(CT1/CT2) |
| 42a0H | float | IoT/angle/V2V1 | Phase angle of V2 to V1 V1/V2(3$4) | F25 | angle=Rx/10 |
| 42a1H | float | IoT/angle/V3V1 | Phase angle of V3 to V1 V1/V3(3$4) | F25 | angle=Rx/10 |
| 42a2H | float | IoT/angle/I1V1 | Phase angle of I1 to V1 V1/I1(3$4) | F25 | angle=Rx/10 |
| 42a3H | float | IoT/angle/I2V1 | Phase angle of I2 to V1 V1/I2(3$4) | F25 | angle=Rx/10 |
| 42a4H | float | IoT/angle/I3V1 | Phase angle of I3 to V1 V1/I3(3$4) | F25 | angle=Rx/10 |

### The code correspond of the type of relationship beteween the real value and the value which is transmited, F1 is in the documentation corresponding to "Numerical value equals to communication value" 
### F25 : Phase angle = Rx/10 (in degree)












