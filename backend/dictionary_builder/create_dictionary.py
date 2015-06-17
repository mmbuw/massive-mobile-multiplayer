import sys

## Command line parameters
# 1 - name of input file (usually /usr/include/linux/input.h)
# 2 - name of output file
# 3 - name of configuration file

def start():

  event_file_name = sys.argv[1]
  output_file_name = sys.argv[2]
  config_file_name = sys.argv[3]

  event_file = open(event_file_name, 'r')
  output_file = open(output_file_name, 'w+')
  config_file = open(config_file_name, 'r')

  output_file.write('//Automatically created on every build\n')
  output_file.write('\n')
  output_file.write('#ifndef EVENT_DICTIONARY_HPP\n')
  output_file.write('#define EVENT_DICTIONARY_HPP\n')
  output_file.write('\n')
  output_file.write('#include <linux/input.h>\n')
  output_file.write('#include <map>\n')
  output_file.write('#include <string>\n')
  output_file.write('\n')
  output_file.write('const std::string configuration_file("' + config_file_name + '");\n')
  output_file.write('std::map<std::string, int> eventDictionary;\n')
  output_file.write('\n')
  output_file.write('void fillDictionary()\n')
  output_file.write('{\n')

  output_file.write('    eventDictionary["EV_SYN"] = EV_SYN;\n')

  for line in config_file:
    if line.startswith("type") or line.startswith("event"):
      line = line.replace("\n", '')
      line = line.replace("\t", ' ')
      splitted_line = line.split(" ")
      output_file.write('    eventDictionary["' + splitted_line[1] + '"] = ' + splitted_line[1] + ';\n')

  output_file.write('}\n')
  output_file.write('\n')
  output_file.write('#endif //EVENT_DICTIONARY_HPP')



if __name__ == '__main__':
  start()