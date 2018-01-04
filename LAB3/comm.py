import sys, string, locale
from optparse import OptionParser

class comm:
    def __init__(self, filename1, filename2):
        if filename1 == "-":
            f1 = sys.stdin.readlines()
            f2 = open(filename2, 'r')
        elif filename2 == "-":
            f2 = sys.stdin.readlines()
            f1 = open(filename1, 'r')
        else:
            f1 = open(filename1, 'r')
            f2 = open(filename2, 'r')

        self.line1 = f1.readlines()
        self.line2 = f2.readlines()
        self.list1 = []
        self.list2 = []
        self.list3 = []
        f1.close()
        f2.close()

    def addLine(self, file):
        for i in range(len(file)):
            if file[i] == '':
                file[i] = '\n'
            if file[i].count(' ') >= 1 and file[i].isspace():
                file[i] = '\n' * file[i].count(' ')

    def unsortedComparison(self):
        self.addLine(self.line1)
        self.addLine(self.line2)
        for i in range(len(self.line1)):
            for j in range(len(self.line2)):
                if self.line1[i] == self.line2[j]:
                    self.list3.append(self.line1[i])
                    self.list1.append("          ")
                    self.list2.append("          ")
                    self.line2[j] = ""
                    temp = True
                    break
                else:
                    temp = False

            if temp == False:
                self.list1.append(self.line1[i])
                self.list2.append("")
                self.list3.append("")

        space = ["          "] * len(self.line2)
        self.list1 += space
        self.list2 += self.line2
        self.list3 += space

    def isSorted(self, file, fileNum):
        for i in range(len(file) - 1):
            if locale.strcoll(file[i],file[i+1]) > 0:
                print("%s not sorted") % file
                return False
        return True

    def sortedComparison(self):
        if self.isSorted(self.line1, 1) and self.isSorted(self.line2, 2):
            self.addLine(self.line1)
            self.addLine(self.line2)
            i = j = 0

            while i < len(self.line1) and j < len(self.line2):
                if self.line1[i] == self.line2[j]:
                    self.list3.append(self.line1[i])
                    self.list1.append("          ")
                    self.list2.append("          ")
                    self.line1[i] = " "
                    self.line2[j] = " "
                    i += 1
                    j += 1
                elif self.line2[j] < self.line1[i]:
                    self.list2.append(self.line2[j])
                    self.list1.append("          ")
                    self.list3.append("          ")
                    self.line2[j] = " "
                    j += 1
                elif self.line1[i] < self.line2[j]:
                    self.list1.append(self.line1[i])
                    self.list2.append("           ")
                    self.list3.append("           ")
                    self.line1[i] = " "
                    i += 1

            if i > j:
                space = ["          "] * len(self.line2)
                self.list1 += space
                self.list2 += self.line2
                self.list3 += space
            else:
                space = ["          "] * len(self.line1)
                self.list1 += self.line1
                self.list2 += space
                self.list3 += space

    def printer(self, filename1, filename2, filename3):
        if filename1 == True:
            self.list1 = [''] * len(self.list1)
        if filename2 == True:
            self.list2 = [''] * len(self.list2)
        if filename3 == True:
            self.list3 = [''] * len(self.list3)
        lastList = []

        for i in range(len(self.list1)):
            lastList.append(self.list1[i] + self.list2[i] + self.list3[i])

        for i in range(len(lastList)):
            for j in range(len(lastList[i])):
                if lastList[i][j] != " ":
                    if lastList[i][j] == "\n":
                        temp = lastList[i]
                        for k in range(len(temp)):
                            if temp[k] == '\n':
                                sys.stdout.write(" ")
                            else:
                                sys.stdout.write(temp[k])
                        print('')
                        break
                    else:
                        print(lastList[i])
                        break
def main():
    locale.setlocale(locale.LC_ALL, 'C')
    version_msg = "%prog 1.0"
    usage_msg = "%prog [-123u] FILE1 FILE2"
    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.add_option("-1", action="store_true", dest="one",
          default = False,
          help="suppress column 1 (lines unique to FILE1)")
    parser.add_option("-2", action="store_true", dest="two",
          default = False,
          help="suppress column 2 (lines unique to FILE2)")
    parser.add_option("-3", action="store_true", dest="three",
          default = False,
          help="suppress column 3 (lines that appear in both files)")
    parser.add_option("-u", action="store_true", dest="unsort",
          default = False,
          help="run comparison on unsorted files line by line." )
    options, args = parser.parse_args(sys.argv[1:])

    one = options.one
    two = options.two
    three = options.three
    unsort = options.unsort

    if len(args) != 2:
        parser.error("missing option arguments")
    f1 = args[0]
    f2 = args[1]

    try:
        compare = comm(f1, f2)
        if unsort == 0:
            compare.unsortedComparison()
        else:
            compare.sortedComparison()
        compare.printer(one, two, three)
    except IOError as e:
        errno = e.errno
        strerror = e.strerror
        parser.error("I/O error({0}): {1}".
                     format(errno, strerror))
if __name__ == "__main__":
        main()
