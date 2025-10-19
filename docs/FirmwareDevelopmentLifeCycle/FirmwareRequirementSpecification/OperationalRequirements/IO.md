
# Inputs/Outputs

> If requirements are simple and I/Os are limited, this section can be merged into **Interfaces**.

 > _Description of Input/Output Data use by a Interfaces & Interaction requirement_ 
 
| Name       | Description                          | Characteristics          | Min          | Default | Max           |
|------------|--------------------------------------|--------------------------|--------------|---------|---------------|
| Temperature| Temperature sensor reading           | Analog, sampled @10Hz    | -40°C        | 25°C    | 125°C         |
| Address    | RAM Memory address                   | Digital 32-bits          | 0x5000_0000  | N/A     | 0x8000_0000   |
| Size       | Number of Byte                       | Digital 32-bits          | 0            | N/A     | UINT32_MAX    |
--- 
