from PIL import Image

def find_sprite_hitbox(image_path, name, block, blockWidth, blockHeight, sizeScaling):
    with Image.open(image_path) as img:

        # Convert image to RGBA if not already
        img = img.convert("RGBA")
        pixels = img.load()

        # Initialize min/max coordinates with opposite values
        min_x, min_y = img.width, img.height
        max_x, max_y = 0, 0

        # Iterate over all pixels to find the bounding box
        for x in range(block * blockWidth, block * blockWidth + blockWidth):
            for y in range(blockHeight):
                r, g, b, a = pixels[x, y]
                if a > 0:  # Alpha channel is not transparent
                    min_x = min(min_x, x - block * blockWidth)
                    min_y = min(min_y, y)
                    max_x = max(max_x, x - block * blockWidth)
                    max_y = max(max_y, y)

        # Check if any non-transparent pixel was found 
        print("—————————————————————————————————————")
        print(f"{name} hitbox coordinates and size: ")
        print(f"Box coordinates x, y:  {block * blockWidth}, {0}. w, h: {blockWidth}, {blockHeight}")
        print(f"Scaled box coordinates x, y:  {sizeScaling * block * blockWidth}, {0}. w, h: {sizeScaling * blockWidth}, {sizeScaling * blockHeight}")
        if min_x < max_x and min_y < max_y:
            print("Min x, Min y: ", sizeScaling * min_x, sizeScaling * min_y)
            print("Width, height: ", sizeScaling * (max_x - min_x), sizeScaling * (max_y - min_y))
            print("Center x, center y: ", sizeScaling * min_x + sizeScaling * (max_x - min_x) // 2, sizeScaling * min_y + sizeScaling * (max_y - min_y) // 2)
        
        print()

# Remember: If the Player has a size scaling of 2, multiply these offsets by 2.
find_sprite_hitbox("../graphics/player/knight/Idle.png", "Knight idle", 0, 128, 64, 2)
find_sprite_hitbox("../graphics/player/knight/Attacks.png", "Knight attack", 3, 128, 64, 2)
find_sprite_hitbox("../graphics/enemies/slimes/Blue_Slime/Idle.png", "Slime idle", 0, 128, 128, 1)
