import sys

## Command line parameters
# 1 - name of input file (usually /usr/include/linux/input.h)
# 2 - name of output file

def start():

  event_file_name = sys.argv[1]
  output_file_name = sys.argv[2]

  event_file = open(event_file_name, 'r')
  output_file = open(output_file_name, 'w+')

  output_file.write('//Automatically created on every build\n')
  output_file.write('\n')
  output_file.write('#ifndef EVENT_DICTIONARY_HPP\n')
  output_file.write('#define EVENT_DICTIONARY_HPP\n')
  output_file.write('\n')
  output_file.write('#include <linux/input.h>\n')
  output_file.write('#include <map>\n')
  output_file.write('#include <string>\n')
  output_file.write('\n')
  output_file.write('std::map<std::string, int> event_dictionary;\n')
  output_file.write('\n')

  start_parsing_events = False

  for line in event_file:

    # start parsing the file from the point where events are listed
    if start_parsing_events == False:
      if "Event types" in line:
        start_parsing_events = True

    else:

      if line.startswith('#define'):
        line = line.replace('\n', '')
        line = line.replace('\t', ' ')
        splitted_line = line.split(' ')
        splitted_line = [value for value in splitted_line if value != '']
        splitted_line = splitted_line[0:3]

        output_file.write('event_dictionary["' + splitted_line[1] + '"] = ' + splitted_line[1] + ';\n')

  output_file.write('\n')
  output_file.write('#endif //EVENT_DICTIONARY_HPP')



if __name__ == '__main__':
  start()