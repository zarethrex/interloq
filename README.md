# Interlocking Library

This library allows the user to define an interlocking setup for railway signalling with the option to load such a setup from a YAML file.

## YAML Specification

The configuration consists of a name for the setup, and the definition for the lever frame. In this definition a single lever is represented as:

```yaml
- id: 23
  name: Junction West Outer Home
  type: StopSignal
```

where type is one of the enumeration values:

```c++
enum class LeverType {
  StopSignal,
  DistantSignal,
  Points,
  PointLock,
  Crossing,
  Detonator,
  Spare,
};
```

If operation of the lever depends on the states of other levers we define dependencies on that lever:

```yaml
- id: 23
  name: Junction West Outer Home
  type: StopSignal
  dependencies:
    - target: 12
      required_state: false
```

A dependency consists of a `target` lever, and its `required_state`. For cases where only one of two or more required states is needed the `alt_choices` key can be used.

The following example demonstrates all of these concepts. In this case operation of the two Up home signals representing a main and branch line, depends on the state of the points. The clearing of the associated distant signal then depends on either home signal being clear.

```yaml
---
name: Basic Example
leverframe:
  - id: 1
    name: Up Main Home
    type: StopSignal
    dependencies:
      - target: 3
        required_state: false
  - id: 2
    name: Down Main Home
    type: StopSignal
  - id: 3
    name: Up Junction
    type: Points
    dependencies:
      - target: 1
        required_state: false
  - id: 4
    name: Up Branch
    type: StopSignal
    dependencies:
      - target: 3
        required_state: true
  - id: 5
    name: Up Distant
    type: DistantSignal
    dependencies:
      - target: 1
        required_state: true
        alt_choices:
          - 4
      - target: 4
        required_state: true
```
