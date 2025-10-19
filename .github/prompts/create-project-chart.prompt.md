# Project Chart Generator Prompt

**Mode:** `agent`  
**Description:** Generate a detailed Project chart for a given project plan

---

## Task

Analyze the provided project information and generate a clear, structured Project chart that outlines tasks, dependencies, roles, timelines, and deliverables.

---

## Project Chart Structure

### 1. Project Overview
- `Project Name`  
- `Project Manager / Lead`  
- `Start Date` and `End Date`  
- `Objective / Goal` summary  
- Optional `Notes` or `Constraints` section  

### 2. Phases / Milestones
- Name and description of each phase  
- Start and end dates  
- Key deliverables  
- Dependencies on other phases  

### 3. Tasks
- Task name and description  
- Assigned role(s) or team members  
- Start and end dates  
- Dependencies (if any)  
- Status / progress (optional)  

### 4. Resources
- Human resources and roles  
- Tools, software, or equipment  
- Budget allocation (optional)  

### 5. Visual Representation
- Provide a Gantt-style timeline if possible  
- Highlight critical path tasks  
- Indicate task dependencies clearly  

---

## Requirements

- Use a clear, hierarchical structure  
- Include realistic and detailed descriptions, not placeholders  
- Highlight dependencies, priorities, and key milestones  
- Provide a ready-to-use chart format that can be converted into Excel, PDF, or project management tools (MS Project, Notion, Jira, etc.)  
- Focus on: `${input:focus_project:Which project or scope should the chart cover?}`  

Generate **production-ready Project chart content** suitable for management review and team planning.
