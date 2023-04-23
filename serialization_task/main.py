from enum import Enum
import struct

class Alignment(Enum):
    HORIZONTAL = 1
    VERTICAL = 2

class Widget():

    def __init__(self, parent):
        self.parent = parent
        self.childrens = []
        if self.parent is not None:
            self.parent.add_children(self)

    def add_children(self, children: "Widget"):
        self.childrens.append(children)

    def to_binary(self):
        bclassname = self.__class__.__name__.encode()
        bparent = b""
        if self.parent is not None:
            bparent = self.parent.__class__.__name__.encode()
        result = struct.pack("i", len(bclassname)) + bclassname + struct.pack("i", len(bparent)) + bparent
        data = b""
        match self.__class__.__name__:
            case 'Layout':
                data += struct.pack("i", len(str(self.alignment))) + str(self.alignment).encode()
            case "LineEdit":
                data += struct.pack("i", self.max_length)
            case "ComboBox":
                data += struct.pack("i", len(self.items))
            case "MainWindow":
                data += struct.pack("i", len(self.title)) + self.title.encode()
        for child in self.childrens:
            bnode = child.to_binary()
            data += bnode
        result += struct.pack("i", len(data)) + data
        return result

    @classmethod
    def from_binary(cls, data, parent=None):
        len_classname = struct.unpack("i", data[:4])[0]
        cur_size = 4
        classname = data[cur_size:cur_size + len_classname].decode()
        cur_size += len_classname
        len_parent = struct.unpack("i", data[cur_size:cur_size + 4])[0]
        cur_size += 4
        parent = None
        if len_parent > 0:
            parent = data[cur_size:cur_size + len_parent].decode()
        cur_size += len_parent
        len_children = struct.unpack("i", data[cur_size:cur_size + 4])[0]
        cur_size += 4
        print(parent)
        input()
        root = cls(parent)
        print(root)
        input()
        data_children = data[cur_size:]
        cursor = 0
        while cursor < len_children:
            node, node_cursor = Widget.from_binary(data_children[cursor:], parent=root)
            root.add_children(node)
            cursor += node_cursor
        return root, cur_size

    def __str__(self):
        return f"{self.__class__.__name__}{self.childrens}"

    def __repr__(self):
        return str(self)

class MainWindow(Widget):

    def __init__(self, title: str):
        super().__init__(None)
        self.title = title

class Layout(Widget):

    def __init__(self, parent, alignment: Alignment):
        super().__init__(parent)
        self.alignment = alignment

class LineEdit(Widget):

    def __init__(self, parent, max_length: int=10):
        super().__init__(parent)
        self.max_length = max_length

class ComboBox(Widget):

    def __init__(self, parent, items):
        super().__init__(parent)
        self.items = items

app = MainWindow("Application")
layout1 = Layout(app, Alignment.HORIZONTAL)
layout2 = Layout(app, Alignment.VERTICAL)

edit1 = LineEdit(layout1, 20)
edit2 = LineEdit(layout1, 30)

# box1 = ComboBox(layout2, [1, 2, 3, 4])
# box2 = ComboBox(layout2, ["a", "b", "c"])

print(app)

bts = app.to_binary()
print(bts)
print(f"Binary data length {len(bts)}")

new_app = MainWindow.from_binary(bts)
print(new_app)