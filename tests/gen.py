import sys, ccparser, random

try:
    (namespaces, global_functions) = ccparser.parse_file("dummy.cpp")
except Exception, e:
    print "Error: " + str(e)
    sys.exit(1)

def random_color():
    return 'Color(' + str(random.randint(0, 125)) + ", " + str(random.randint(0, 125)) + ", " + str(random.randint(0, 125)) + ", " + str(random.randint(0, 125)) + ')'

tw = namespaces["tw"]
fun = namespaces["extra"]

tb = ccparser.TestBuilder()
tb.template_map = {
    "DrawingType" : "DrawingRGB24",
    "PixelType" : "uint8_t",
    "PixelLength" : "3",
    "DataType" : "uint8_t",
    "LayersImageType" :  "Image<uint8_t>",
    "LayersImageType &" :  "Image<uint8_t>",
}

tb.type_map["Color"] = random_color()
tb.type_map["ImageType"] = "test_Image"
tb.type_map["ImageType &"] = "test_Image"
tb.type_map["Image<DataType> &"] = "test_Image"
tb.type_map["DataType"] = tb.random_int(0, 255)
tb.type_map["Drawing<DrawingType> &"] = "test_Drawing"
tb.type_map["LayersImageType &"] = "test_Image"
tb.type_map["const Image<DataType> &"] = "test_Image"
tb.type_map["tw::Point"] = "tw::Point(10, 10)"
tb.type_map["Point"] = "Point(10, 10)"
tb.type_map["Rectangle"] = "Rectangle(0, 0, 10, 10)"
tb.type_map["tw::Rectangle"] = "Rectangle(0, 0, 10, 10)"


tb.type_map["tw::line_join_style"] = "miter_join"
tb.type_map["tw::line_cap_style"] = "round_cap"
tb.type_map["tw::filling_rule"]  = "tw::fill_non_zero"
tb.type_map["list<ItemType> &"] = "test_list"
tb.type_map["list<ItemType>"]  = "test_list"
tb.type_map["Image<uint16_t> &"] = "test_Imagew"
tb.type_map["Image<float> &"] = "test_Imagef"
tb.type_map["ImageSizeType"] = tb.random_int(0, 500)

tb.expect_throws.append("test_Layers.operator[]")
tb.expect_throws.append("test_Layers.image")
tb.expect_throws.append("test_Layers.point")
tb.expect_throws.append("openTIFF")
tb.expect_throws.append("imread")
tb.expect_throws.append("test_Image.component")

tb.ignore_functions.extend(["saveTIFF", "imwrite", "rewind", "invertPolygon", "getCommand", "push_back", "test_Layers.operator+=", "test_maybe.check", "test_list.operator+=", "test_Image.scale"])

print tb.gen_for_ns([tw, fun])


