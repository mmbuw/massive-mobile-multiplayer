import os
import sys

# Command line parameters
# 1 - name of configuration file
# 2 - name of output file


def start():

    # get filenames and load files
    config_file_name = sys.argv[1]
    config_file_abs_path = os.path.abspath(config_file_name)
    output_file_name = sys.argv[2]

    config_file = open(config_file_name, 'r')
    output_file = open(output_file_name, 'w+')

    # write static code to output file
    output_file.write('//Automatically created on every build\n')
    output_file.write('\n')
    output_file.write('#ifndef EVENT_DICTIONARY_HPP\n')
    output_file.write('#define EVENT_DICTIONARY_HPP\n')
    output_file.write('\n')
    output_file.write('#include <linux/input.h>\n')
    output_file.write('#include <map>\n')
    output_file.write('#include <string>\n')
    output_file.write('\n')
    output_file.write('const std::string configuration_file("' +
                      config_file_abs_path + '");\n')
    output_file.write('std::map<std::string, int> eventDictionary;\n')
    output_file.write('\n')
    output_file.write('void fillDictionary()\n')
    output_file.write('{\n')

    output_file.write('    eventDictionary["EV_SYN"] = EV_SYN;\n')

    # parse configuration file and generate dictionary entried
    for line in config_file:
        if line.startswith("type") or line.startswith("event"):

            line = line.replace("\n", '')
            line = line.replace("\t", ' ')
            splitted_line = line.split(" ")
            output_file.write('    eventDictionary["' + splitted_line[1] +
                              '"] = ' + splitted_line[1] + ';\n')

            # check if an alias is present
            if len(splitted_line) > 2:
                output_file.write('    eventDictionary["' +
                                  splitted_line[2] + '"] = ' +
                                  splitted_line[1] + ';\n')

    # write static end code to output file
    output_file.write('}\n')
    output_file.write('\n')
    output_file.write('#endif //EVENT_DICTIONARY_HPP')


if __name__ == '__main__':
    start()
