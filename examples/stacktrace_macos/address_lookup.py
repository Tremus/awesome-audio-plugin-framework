import sys
import subprocess

def do_usage():
    print("Usage: python3 address_lookup.py /path/to/symbols.dSYM stacktrace_<unixtimestamp>.txt")
    sys.exit()

if (len(sys.argv) != 3):
    do_usage()

symbols_path = sys.argv[1]
stacktrace_path = sys.argv[2]
if not symbols_path.endswith(".dSYM") and not symbols_path.endswith(".dSYM/"):
    print("Argument 1 not a .dSYM file (actually a folder)")
    do_usage()
if not stacktrace_path.endswith(".txt"):
    print("Argument 2 not a .txt file")
    do_usage()

file=open(stacktrace_path,'r') 
lines = file.readlines()
if not lines[0].startswith("Binary"):
    print("Could not recognise contents in file")
    sys.exit()

line_1_parts = lines[0].split("/")

binary_name = line_1_parts[-1]
binary_name = binary_name.replace("\n", "")
binary_name = binary_name.replace("\r", "")

line_2_parts = lines[1].split(" ")

base_address = line_2_parts[-1]
base_address = base_address.replace("\n", "")
base_address = base_address.replace("\r", "")

print("Looking up symbols for", binary_name, "at address", base_address)

for line in lines[2:]:

    line_n_parts = line.split(' ')
    line_n_parts = [s for s in line_n_parts if len(s)]

    if line_n_parts[1] == binary_name:
        local_address = line_n_parts[2]
        # atos -o "./build/make_stacktrace.dSYM" -l 0xabcdef 0xabcdef123455678
        result = subprocess.run(["atos", "-o", symbols_path, "-l", base_address, local_address], capture_output=True, text=True)
        # print(result.stdout)
        line += " - "
        line +=  result.stdout

    line = line.replace("\n", "")
    line = line.replace("\r", "")
    line += "\n"

    print(line, end='')
    # print(line_n_parts, end='')
    # print(line_n_parts)
    # print(line.replace("  ", " "), end="")
