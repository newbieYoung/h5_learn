/**
 * 卷积
 * ker      卷积核
 * kerRows  卷积核行数
 * kerCols  卷积核列数
 * mat      图片矩阵
 * matRows  矩阵行数
 * matCols  矩阵列数
 * ----
 * 对于图片来说行数即高度、列数即宽度
 */
float * convolution(float ker[], int kerRows, int kerCols, float mat[], int matRows, int matCols){
  int len = 4;
  int matLen = matRows * matCols * len;

  //暂不考虑非奇数尺寸卷积核
  if (kerRows % 2 != 1 || kerCols % 2 != 1) {
    return 0;
  }

  //一般而言卷积核所有元素之和等于1；否则大于1时生成的图片亮度会增加，小于1时生成的图片亮度会降低
  float sums[4] = {0.0, 0.0, 0.0, 0.0};
  
  for (int z = 0; z < len; z++) {
    for (int i = 0; i < kerRows; i++) {
      for (int j = 0; j < kerCols; j++) {
        int no = (i * kerCols  + j) * len;
        sums[z] += ker[no + z];
      }
    }
  }

  for (int z = 0; z < len; z++) {
    for (int i = 0; i < kerRows; i++) {
      for (int j = 0; j < kerCols; j++) {
        int no = (i * kerCols  + j) * len;
        ker[no + z] = ker[no + z] / sums[z];
      }
    }
  }

  //卷积核中心
  int cRow = (kerRows - 1) / 2;
  int cCol = (kerCols - 1) / 2;

  //卷积计算
  for (int r = 0; r < matRows; r++) {
    int rs = r - cRow;
    for (int c = 0; c < matCols; c++) {
      int cs = c - cCol;
      int noCur = (r * matCols + c) * len;
      for (int z = 0; z < len; z++) {
        float sum = 0.0;
        if (z == len - 1) {
          //透明通道不参与计算
          sum = mat[noCur + z];
        } else {
          for (int i = 0; i < kerRows; i++) {
            int r1 = rs + i;
            for (int j = 0; j < kerCols; j++) {
              int c1 = cs + j;

              int n1 = (r1 * matCols + c1) * len;
              float v1 = 0.0; //超出范围使用常量0代替
              if (n1 + z >= 0 && n1 + z <= matLen - 1) {
                v1 = mat[n1 + z];
              }

              int r2 = kerRows - 1 - i; //卷积核旋转180度
              int c2 = kerCols - 1 - j;

              int n2 = (r2 * kerCols + c2) * len;
              float v2 = ker[n2 + z];

              sum += v1 * v2;
            }
          }
        }
        mat[noCur + z] = sum;
      }
    }
  }

  return mat;
}