# EPIC-001: Software Development Environment Setup 

## User Story

As a developer, I want a properly configured software development environment so that I can build, test, and debug the project consistently across all machines, including Raspberry Pi, macOS, Windows, or a Docker image.

## Acceptance Criteria

- [ ] Update `README.md` to list all required tools and their purpose, including versions and platform notes
- [ ] Provide `DevEnvSetup.sh` script to install all tools on supported platforms
- [ ] Provide `DevEnvCheck.sh` script to verify tool installation and versions
- [ ] Provide `DevEnvUninstall.sh` script to uninstall all installed tools
- [ ] Provide a `tools.json` or `tools.csv` file that defines all tools, versions, platform support, and optional/mandatory status

## Deliverables 

**README.md** 
- Lists all required tools and versions 
- Provides usage instructions for setup, check, and uninstall scripts
- Includes example commands for each platform (Raspberry Pi, macOS, Windows, Docker)

**Scripts** 
 - `DevEnvSetup.sh`: installs all tools listed in the JSON/CSV file for the detected platform 
 - `DevEnvCheck.sh`: checks installed tools against required versions 
 - `DevEnvUninstall.sh`: removes installed tools 
3. **Tools Definition File** 
 - `tools.json` or `tools.csv` 
 - Contains: 
 ```json
 [
 {
 "name": "gcc",
 "version": "12.2",
 "platform": ["raspberry", "macos", "windows", "docker"],
 "mandatory": true
 },
 {
 "name": "cmake",
 "version": "3.26",
 "platform": ["raspberry", "macos", "windows", "docker"],
 "mandatory": true
 }
 ]
 ``` 
 - Scripts and README will consume this file to automate setup, check, and uninstall 
## Notes 
- Branch name: `EPIC-001-Software-Dev-Env-Setup` 
- Scripts should automatically detect the OS/platform and run the appropriate commands 
- Error handling: scripts should fail gracefully if a tool cannot be installed, with clear instructions 
- Optional tools (e.g., `doxygen`) should be clearly marked in the tools file 
## Example Usage 
### Setup
```bash
chmod +x DevEnvSetup.sh
./DevEnvSetup.sh
```
### Check
```bash
chmod +x DevEnvCheck.sh
./DevEnvCheck.sh
```
### Uninstall
```bash
chmod +x DevEnvUninstall.sh
./DevEnvUninstall.sh
```