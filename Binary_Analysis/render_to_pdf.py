from graphviz import Source

import sys
s=Source.from_file(sys.argv[1])
s.render(view=False)

