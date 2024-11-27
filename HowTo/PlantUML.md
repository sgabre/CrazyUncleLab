# Structure Diagrams

## Class Diagram

Hierarchical State Pattern

``` shell
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


``` shell
dqsd qdqsd
```
