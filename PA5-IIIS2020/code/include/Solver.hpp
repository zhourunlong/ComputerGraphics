#include "image.hpp"
#include <cmath>

float dist(const Vector3f& p, const Vector3f& q) {
	// TODO (PA5): ��������p��q֮���L1����
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
        // ����ͼƬ������ڴ�
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
            // ɾ��������
			int wCut = w * rate + 0.5;  // ��Ҫɾ��������
			for (int i = 0; i < wCut; i++) {
				computeGradientMap(w-i, h); // ����ͼ���ݶ�
				cutOneColumn(w-i, h); // ɾ��һ��
			}
			// �������ͼ��
			img_out.reshape(w-wCut, h);
			for (int y = 0; y < h; y++)
				for (int x = 0; x < w - wCut; x++)
					img_out.SetPixel(x, y, img.GetPixel(x, y));
		}
		else {
            // ɾ��������
			int hCut = h * rate + 0.5; // ��Ҫɾ��������
			for (int i = 0; i < hCut; i++) {
				computeGradientMap(w, h - i); // ����ͼ���ݶ�
				cutOneRow(w, h - i); // ɾ��һ��
			}
			// �������ͼ��
			img_out.reshape(w, h - hCut);
			for (int y = 0; y < h - hCut; y++)
				for (int x = 0; x < w; x++)
					img_out.SetPixel(x, y, img.GetPixel(x, y));
		}
		img_out.SaveBMP(fileOut);
	}

private:
	void computeGradientMap(int ww, int hh) {
        // ��ǰ���ַ�������x�����ݶ�gradx��y�����ݶ�grady
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
        // ɾ��ͼ����������С��һ�нӷ�
        // TODO (PA5): ���Ƽ����ۻ���������dp��ǰ��λ������pred





		// �����һ���ҵ����Žӷ��ϵ�����λ��
		float minVal = 1e10;
		int pos = 0;
		for (int x = 0; x < ww; x++)
			if (minVal > dp[hh - 1][x]) {
				pos = x;
				minVal = dp[hh - 1][x];
			}
		for (int y = hh - 1; y >= 0; y--) {
            // (y, pos)�Ǵ�ɾ�����������꣬�����ұߵ�������������
			for (int x = pos; x < ww - 1; x++)
				img.SetPixel(x, y, img.GetPixel(x + 1, y));
			// ����ǰ����¼pred����pos
			pos += pred[y][pos];
		}
	}

	void cutOneRow(int ww, int hh) {
        // ɾ��ͼ����������С��һ�нӷ�
        // TODO (PA5): ���Ƽ����ۻ���������dp��ǰ��λ������pred





		// �����һ���ҵ����Žӷ��ϵ�����λ��
		float minVal = 1e10;
		int pos = 0;
		for (int y = 0; y < hh; y++)
			if (minVal > dp[y][ww - 1]) {
				pos = y;
				minVal = dp[y][ww - 1];
			}
		for (int x = ww - 1; x >= 0; x--) {
            // (pos, x)�Ǵ�ɾ�����������꣬�����ұߵ�������������
			for (int y = pos; y < hh - 1; y++)
				img.SetPixel(x, y, img.GetPixel(x, y + 1));
			// ����ǰ����¼pred����pos
			pos += pred[pos][x];
		}
	}

	Image img, img_out; // ����ͼƬ�����ͼƬ
	float** gradx; // x�����ͼ���ݶ�
	float** grady; // y�����ͼ���ݶ�
	float** dp; // ���ڵ����ۻ������ܺ͵����飬��Ӧ��introduction�е�M
	float** pred; // pred[y][x]���ڼ�¼(y,x)���ڽӷ��ǰһ��λ�õ�ƫ������ȡֵ��Χ��{-1, 0, 1}
	int h, w; // ͼƬ�ĳ���
	bool isWidth; // =true��ʾ����������=false��ʾ��������
};