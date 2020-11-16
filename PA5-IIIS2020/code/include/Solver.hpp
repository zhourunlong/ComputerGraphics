#include "image.hpp"
#include <cmath>

float dist(const Vector3f& p, const Vector3f& q) {
	// TODO (PA5): 计算向量p和q之间的L1距离
}

class Solver {
public:
	Solver() {
		gradx = NULL;
		grady = NULL;
	}

	~Solver() {
		if (gradx != NULL) {
			for (int y = 0; y < h; y++) 
				if (gradx[y]) delete[] gradx[y];
			delete[] gradx;
		}
		if (grady != NULL) {
			for (int y = 0; y < h; y++)
				if (grady[y]) delete[] grady[y];
			delete[] grady;
		}
		if (dp != NULL) {
			for (int y = 0; y < h; y++)
				if (dp[y]) delete[] dp[y];
			delete[] dp;
		}
		if (pred != NULL) {
			for (int y = 0; y < h; y++)
				if (pred[y]) delete[] pred[y];
			delete[] pred;
		}
	}

	void process(char* fileIn, char* fileOut, bool isWidth, float rate) {
        // 读入图片与分配内存
		img.LoadBMP(fileIn);
		h = img.Height();
		w = img.Width();
		gradx = new float*[h];
		grady = new float*[h];
		dp = new float*[h];
		pred = new float*[h];
		for (int y = 0; y < h; y++) {
			gradx[y] = new float[w];
			grady[y] = new float[w];
			dp[y] = new float[w];
			pred[y] = new float[w];
		}
		
		if (isWidth) {
            // 删除若干列
			int wCut = w * rate + 0.5;  // 需要删除的列数
			for (int i = 0; i < wCut; i++) {
				computeGradientMap(w-i, h); // 计算图像梯度
				cutOneColumn(w-i, h); // 删除一列
			}
			// 构造输出图像
			img_out.reshape(w-wCut, h);
			for (int y = 0; y < h; y++)
				for (int x = 0; x < w - wCut; x++)
					img_out.SetPixel(x, y, img.GetPixel(x, y));
		}
		else {
            // 删除若干行
			int hCut = h * rate + 0.5; // 需要删除的行数
			for (int i = 0; i < hCut; i++) {
				computeGradientMap(w, h - i); // 计算图像梯度
				cutOneRow(w, h - i); // 删除一行
			}
			// 构造输出图像
			img_out.reshape(w, h - hCut);
			for (int y = 0; y < h - hCut; y++)
				for (int x = 0; x < w; x++)
					img_out.SetPixel(x, y, img.GetPixel(x, y));
		}
		img_out.SaveBMP(fileOut);
	}

private:
	void computeGradientMap(int ww, int hh) {
        // 用前向差分方法计算x方向梯度gradx和y方向梯度grady
		Vector3f p, q;
		for (int y = 0; y < hh; y++) {
			for (int x = 1; x < ww; x++) {
				p = img.GetPixel(x, y);
				q = img.GetPixel(x - 1, y);
				float diff = dist(p, q);
				gradx[y][x] = diff;
			}
			gradx[y][0] = gradx[y][1];
		}
		for (int x = 0; x < ww; x++) {
			for (int y = 1; y < hh; y++) {
				p = img.GetPixel(x, y);
				q = img.GetPixel(x, y - 1);
				float diff = dist(p, q);
				grady[y][x] = diff;
			}
			grady[0][x] = grady[1][x];
		}
	}

	void cutOneColumn(int ww, int hh) {
        // 删除图像中能量最小的一列接缝
        // TODO (PA5): 递推计算累积能量数组dp和前驱位置数组pred





		// 在最后一行找到最优接缝上的像素位置
		float minVal = 1e10;
		int pos = 0;
		for (int x = 0; x < ww; x++)
			if (minVal > dp[hh - 1][x]) {
				pos = x;
				minVal = dp[hh - 1][x];
			}
		for (int y = hh - 1; y >= 0; y--) {
            // (y, pos)是待删除的像素坐标，在它右边的像素依次左移
			for (int x = pos; x < ww - 1; x++)
				img.SetPixel(x, y, img.GetPixel(x + 1, y));
			// 根据前驱记录pred更新pos
			pos += pred[y][pos];
		}
	}

	void cutOneRow(int ww, int hh) {
        // 删除图像中能量最小的一列接缝
        // TODO (PA5): 递推计算累积能量数组dp和前驱位置数组pred





		// 在最后一列找到最优接缝上的像素位置
		float minVal = 1e10;
		int pos = 0;
		for (int y = 0; y < hh; y++)
			if (minVal > dp[y][ww - 1]) {
				pos = y;
				minVal = dp[y][ww - 1];
			}
		for (int x = ww - 1; x >= 0; x--) {
            // (pos, x)是待删除的像素坐标，在它右边的像素依次左移
			for (int y = pos; y < hh - 1; y++)
				img.SetPixel(x, y, img.GetPixel(x, y + 1));
			// 根据前驱记录pred更新pos
			pos += pred[pos][x];
		}
	}

	Image img, img_out; // 输入图片与输出图片
	float** gradx; // x方向的图像梯度
	float** grady; // y方向的图像梯度
	float** dp; // 用于递推累积能量总和的数组，对应于introduction中的M
	float** pred; // pred[y][x]用于记录(y,x)所在接缝的前一个位置的偏移量，取值范围是{-1, 0, 1}
	int h, w; // 图片的长宽
	bool isWidth; // =true表示减少列数，=false表示减少行数
};