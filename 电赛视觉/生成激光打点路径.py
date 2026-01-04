import matplotlib.pyplot as plt
from collections import defaultdict


def save_contour_math_coords(contour, filename):
    """将轮廓坐标以数学坐标系保存到txt文件"""
    # 计算最大y值（用于坐标转换）
    max_y = max(x for x, y in contour) if contour else 0

    with open(filename, 'w') as f:
        # 写入文件头说明
        f.write("# 数学坐标系轮廓点 (x=横轴,y=纵轴,原点在左下角)\n")
        f.write("# 格式: x y\n")

        # 转换并写入每个点
        for x, y in contour:
            math_x = y  # 原y坐标变为数学x坐标（横轴）
            math_y = max_y - x  # 转换y坐标（纵轴）
            f.write(f"{math_x} {math_y}\n")

    print(f"数学坐标系轮廓点已保存到 {filename}")



def read_zero_positions(filename):
    """读取文件并返回所有0的位置坐标"""
    zeros = []
    with open(filename, 'r') as f:
        for x, line in enumerate(f):
            line = line.strip()
            for y, char in enumerate(line):
                if char == '0':
                    zeros.append((x, y))
    return zeros


def build_position_grid(zeros):
    """构建坐标网格字典便于快速查找"""
    grid = defaultdict(dict)
    for x, y in zeros:
        grid[x][y] = True
    return grid


def find_min_xy_point(zeros):
    """找到最小(x,y)坐标的点（最左上角）"""
    return min(zeros, key=lambda p: (p[0], p[1]))


def get_clockwise_neighbors(current, previous):
    """获取顺时针方向的8邻域搜索顺序"""
    x, y = current
    px, py = previous

    # 计算方向向量
    dx, dy = x - px, y - py

    # 8个可能的邻域位置（顺时针顺序）
    all_neighbors = [
        (x, y + 1),  # 右
        (x + 1, y + 1),  # 右下
        (x + 1, y),  # 下
        (x + 1, y - 1),  # 左下
        (x, y - 1),  # 左
        (x - 1, y - 1),  # 左上
        (x - 1, y),  # 上
        (x - 1, y + 1)  # 右上
    ]

    # 根据进入方向确定起始搜索位置
    if dx == 0 and dy == -1:  # 从左进入
        start_index = 0
    elif dx == -1 and dy == -1:  # 从左上方进入
        start_index = 1
    elif dx == -1 and dy == 0:  # 从上进入
        start_index = 2
    elif dx == -1 and dy == 1:  # 从右上方进入
        start_index = 3
    elif dx == 0 and dy == 1:  # 从右进入
        start_index = 4
    elif dx == 1 and dy == 1:  # 从右下方进入
        start_index = 5
    elif dx == 1 and dy == 0:  # 从下进入
        start_index = 6
    elif dx == 1 and dy == -1:  # 从左下方进入
        start_index = 7
    else:  # 初始情况（假设从左边进入）
        start_index = 0

    # 重新排序邻域列表（从start_index开始）
    ordered_neighbors = all_neighbors[start_index:] + all_neighbors[:start_index]
    return ordered_neighbors


def trace_contour_clockwise(grid, start):
    """从起点开始顺时针跟踪轮廓"""
    contour = [start]
    # 初始方向：假设从左边进入
    previous = (start[0], start[1] - 1)
    current = start

    count = 0
    max_iterations = len(grid) * 8  # 安全限制

    while count < max_iterations:
        count += 1
        neighbors = get_clockwise_neighbors(current, previous)

        found = False
        for neighbor in neighbors:
            nx, ny = neighbor
            if nx in grid and ny in grid[nx]:
                # 避免直接回溯（防止2点循环）
                if len(contour) > 1 and neighbor == contour[-2]:
                    continue

                contour.append(neighbor)
                previous = current
                current = neighbor
                found = True
                break

        if not found:
            break

        # 检查是否回到了起点
        if current == start and len(contour) > 2:
            break

    return contour


def is_boundary_point(point, grid):
    """检查一个点是否是边界点"""
    x, y = point
    # 检查8邻域是否有非0点
    for dx in [-1, 0, 1]:
        for dy in [-1, 0, 1]:
            if dx == 0 and dy == 0:
                continue
            nx, ny = x + dx, y + dy
            if nx not in grid or ny not in grid[nx]:
                return True
    return False


def get_main_contour(grid, zeros):
    """获取主轮廓（外轮廓）"""
    # 找到最小(x,y)的边界点作为起点
    boundary_zeros = [p for p in zeros if is_boundary_point(p, grid)]
    if not boundary_zeros:
        return []

    start_point = find_min_xy_point(boundary_zeros)
    contour = trace_contour_clockwise(grid, start_point)

    # 确保轮廓闭合
    if len(contour) > 2 and contour[0] != contour[-1]:
        contour.append(contour[0])

    return contour


def visualize_contour(zeros, contour):
    """可视化轮廓"""
    fig, ax = plt.subplots(figsize=(10, 10))

    # 绘制所有0点
    if zeros:
        x, y = zip(*zeros)
        ax.scatter(y, x, c='blue', alpha=0.3, label='All zeros')

    # 绘制轮廓
    if contour:
        cx, cy = zip(*contour)
        # 闭合轮廓
        if contour[0] != contour[-1]:
            cx += (contour[0][0],)
            cy += (contour[0][1],)
        ax.plot(cy, cx, 'r-', linewidth=2, label='Contour')
        ax.scatter(cy[0], cx[0], c='green', marker='*', s=200, label='Start point')

    ax.invert_yaxis()
    ax.set_xlabel('Column (y)')
    ax.set_ylabel('Row (x)')
    ax.set_title('Main Contour of Zero Pattern (Clockwise)')
    ax.legend()
    ax.grid()
    plt.show()


def output_contour_coordinates(contour):
    """输出轮廓坐标"""
    print("Contour coordinates (clockwise order starting from min(x,y)):")
    for i, (x, y) in enumerate(contour):
        print(f"{i + 1}: ({x}, {y})")


# def main(filename):
#     # 1. 读取所有0的位置
#     zeros = read_zero_positions(filename)
#     print(f"Total zeros: {len(zeros)}")
#
#     # 2. 构建坐标网格
#     grid = build_position_grid(zeros)
#
#     # 3. 获取主轮廓
#     contour = get_main_contour(grid, zeros)
#     print(f"Contour length: {len(contour)} points")
#
#     # 4. 输出坐标
#     output_contour_coordinates(contour)
#
#     # 5. 可视化结果
#     visualize_contour(zeros, contour)
#
#     return zeros, contour

def main(filename):
    # 1. 读取所有0的位置
    zeros = read_zero_positions(filename)
    print(f"Total zeros: {len(zeros)}")

    # 2. 构建坐标网格
    grid = build_position_grid(zeros)

    # 3. 获取主轮廓
    contour = get_main_contour(grid, zeros)
    print(f"Contour length: {len(contour)} points")

    # 4. 输出原始坐标（图像坐标系）
    output_contour_coordinates(contour)

    # 5. 保存数学坐标系坐标
    save_contour_math_coords(contour, "D:/LabelImg/dianzhen/zuobiao.txt")

    # 6. 可视化结果
    visualize_contour(zeros, contour)

    return zeros, contour



# 运行主程序
filename = "D:/LabelImg/dianzhen/tzn.txt"
zeros, contour = main(filename)


