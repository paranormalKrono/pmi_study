import sys
import xml.etree.ElementTree as ET
from collections import defaultdict

# Set console encoding to UTF-8
sys.stdout.reconfigure(encoding='utf-8')

def parse_filters(xml_file):
    hierarchy = defaultdict(dict)
    
    def add_to_hierarchy(path, file):
        current = hierarchy
        parts = path.split('\\')
        for part in parts:
            if part not in current:
                current[part] = defaultdict(dict)
            current = current[part]
        current[file] = None  # Store the file at the leaf level

    conflicts = []
    
    try:
        tree = ET.parse(xml_file)
        root = tree.getroot()
        ns = {'msbuild': 'http://schemas.microsoft.com/developer/msbuild/2003'}

        # Process ClCompile, ClInclude, and None elements
        for item_group in root.findall('msbuild:ItemGroup', ns):
            for item_elem in item_group:
                if 'Include' in item_elem.attrib:
                    file_path = item_elem.attrib['Include']
                    filter_elem = item_elem.find('msbuild:Filter', ns)
                    if filter_elem is not None:
                        filter_name = filter_elem.text
                        add_to_hierarchy(filter_name, file_path)
    except ET.ParseError as e:
        conflicts.append(f"XML Parse Error: {e}")
    except Exception as e:
        conflicts.append(f"General Error: {e}")
    return hierarchy, conflicts

def print_hierarchy(hierarchy, indent=''):
    markdown = ''
    
    items = sorted(hierarchy.items(), key=lambda x: (isinstance(x[1], defaultdict), x[0]))
    
    items_count = len(items)
    for i in range(0, items_count-1):
        (key, value) = items[i]
        markdown += f"{indent}├── {key}\n"
        
        if value:  # If there are subdirectories or files
            if isinstance(value, defaultdict):
                markdown += print_hierarchy(value, indent + '│   ')
            else:
                markdown += f"{indent}│   ├── {key}\n"
                
    (key, value) = items[items_count - 1]
    markdown += f"{indent}└── {key}\n"
    
    if value:  # If there are subdirectories or files
        if isinstance(value, defaultdict):
            markdown += print_hierarchy(value, indent + '    ')
        else:
            markdown += f"{indent}│   └── {key}\n"
            
    return markdown

def main():
    xml_file = 'Project1/Project1.vcxproj.filters'
    hierarchy, conflicts = parse_filters(xml_file)

    if conflicts:
        print("Conflicts encountered:")
        for conflict in conflicts:
            print(conflict)

    if hierarchy:
        markdown_output = print_hierarchy(hierarchy)
        with open('directory_hierarchy.md', 'w', encoding='utf-8') as f:
            f.write("\n<pre>\n")
            f.write(markdown_output)
            f.write("</pre>")
    else:
        print("No hierarchy found.")

if __name__ == "__main__":
    main()