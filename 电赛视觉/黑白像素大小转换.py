from PIL import Image
def resize_image(input_image_path, output_image_path, new_width, new_height):
     image = Image.open(input_image_path)
     resized_image = image.resize((new_width, new_height))
     gray_image = resized_image.convert('L')
     table = []
     for i in range(256):
          if i < maxvalve :
              table.append(0)
          else:
              table.append(1)
     photo = gray_image.point(table, '1')
     with open("D:/LabelImg/dianzhen/tzn.txt", "w") as f:
          for cc in range(new_height):
               for jj in range(new_width):
                    f.write(str(int(photo.getpixel((jj,cc)))))
                    if jj == new_width - 1:
                         f.write("\n")
     f.close()
     photo.save(output_image_path)



input_image_path = 'D:/LabelImg/videopicture/2.jpg'
output_image_path = 'D:/LabelImg/dianzhen/A6.jpg'
new_width = 60
new_height = 60
maxvalve = 100

resize_image(input_image_path, output_image_path, new_width, new_height)

