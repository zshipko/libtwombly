import sys, ccparser, random

try:
    (namespaces, global_functions) = ccparser.parse_file("dummy.cpp")
except Exception, e:
    print "Error: " + str(e)
    sys.exit(1)

def random_color():
    return 'color8(' + str(random.randint(0, 125)) + ", " + str(random.randint(0, 125)) + ", " + str(random.randint(0, 125)) + ", " + str(random.randint(0, 125)) + ')'

tw = namespaces["tw"]
fun = namespaces["extra"]

tb = ccparser.TestBuilder()
tb.template_map = {
    "DrawingType" : "drawing4u",
    "PixelType" : "uint8_t",
    "PixelLength" : "4",
    "ImageType" : "pixel4u",
    "LayersImageType" :  "image4u",
    "LayersImageType &" :  "image4u",
}

tb.type_map["pixel<PixelType, PixelLength>"] = "test_pixel"
tb.type_map["pixel<PixelType, PixelLength> &"] = "test_pixel",
tb.type_map["color8"] = random_color()
tb.type_map["ImageType"] = "test_image"
tb.type_map["ImageType &"] = "test_image"
tb.type_map["image<ImageType> &"] = "test_image"
tb.type_map["PixelType"] = tb.random_int(0, 255)
tb.type_map["drawing<DrawingType> &"] = "test_drawing"
tb.type_map["LayersImageType &"] = "test_image"
tb.type_map["const image<ImageType> &"] = "test_image"
tb.type_map["pixel<PixelType, PixelLength> &"] = "test_pixel2"
tb.type_map["tw::point"] = "tw::point(10, 10)"

tb.type_map["tw::line_join_style"] = "miter_join"
tb.type_map["tw::line_cap_style"] = "round_cap"
tb.type_map["tw::filling_rule"]  = "tw::fill_non_zero"
tb.type_map["list<ItemType> &"] = "test_list"
tb.type_map["list<ItemType>"]  = "test_list"
tb.type_map["image1s &"] = "test_image1s"
tb.type_map["image1f &"] = "test_image1f"
tb.type_map["image3f &"] = "test_image3f"
tb.type_map["image4u &"] = "test_image"

tb.expect_throws.append("test_layers.operator[]")
tb.expect_throws.append("test_layers.image")
tb.expect_throws.append("test_layers.point")
tb.expect_throws.append("openTIFF")
tb.expect_throws.append("imread")

tb.ignore_functions.extend(["saveTIFF", "imwrite", "rewind", "invertPolygon", "getCommand", "push_back", "test_layers.operator+=", "test_maybe.check"])

print tb.gen_for_ns([tw, fun])


