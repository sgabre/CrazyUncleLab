# Structure Diagrams

##

``` PlantUML
@startuml
node MCU [
Microcontroler
----
(STM32F401RE)
]
node JTAG

node BLE[
Bluetooth
----
(IDB0xA1)
]
frame UI {
node  n #aliceblue;line:blue;line.dotted;text:blue
node Button
node Led
}

frame Connectivity {
node  x #aliceblue;line:blue;line.dotted;text:blue
node  y #aliceblue;line:blue;line.dotted;text:blue
}

frame Motor {
node M1 #aliceblue;line:blue;line.dotted;text:blue [
Elevation Motor
] 
node M2 #aliceblue;line:blue;line.dotted;text:blue [
Azimuth Motor
] 
}

MCU =left= JTAG : JTAG
MCU =up= Button : GPIO(PC13)
MCU == Led : GPIO(PA5)
MCU =right= BLE : SPI
MCU =up= M1 : PWM
MCU =up= M2 : PWM
@enduml
```
## Class Diagram

Hierarchical State Pattern

Hierarchical State pattern template proposed by Sunitha and Samuel

``` PlantUML
@startuml
class Events
class StateMachine
class ContextClass
class State
class OrthogonalProperty
class HistoryState
class SimpleState
class CompositeState
class SubState
Events -- StateMachine
StateMachine <|-- ContextClass
ContextClass *-- State
State <|-- HistoryState
State <|-- SimpleState
OrthogonalProperty <|-- CompositeState
State <|-- CompositeState
CompositeState <|-- SubState
Events : +setSignal()
StateMachine : +transition(State)
ContextClass : -ActiveState
ContextClass : -ShallowHistory[]:state
ContextClass : -dispatch(Events)
State : -dispatch(Events)
SimpleState : -dispatch(Events)
CompositeState : -dispatch(Events)
@enduml
```

# Behavior diagrams

## State Machine Diagrams


``` PlantUML
@startuml
[*] --> A
A --> B : 1, 4
B --> A : 2, 3
B --> C : 5
C --> D : 7
D --> C : 6, 8
C -right-> E : 9
E -left> C : 10
D -right-> F : 12, 13
F -left> D : 14
F -right-> H : 22
H -left> E : 21
F -up-> E : 17, 18
G -left-> A : 19
G --> B : 20
E --> A : 15, 16
D --> G : 23
@enduml
```
