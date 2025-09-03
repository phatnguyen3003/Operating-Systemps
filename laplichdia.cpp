#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int head;
vector<int> requests;

void docdulieu(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Khong the mo file input!\n";
        exit(1);
    }
    file >> head;
    int r;
    while (file >> r) {
        requests.push_back(r);
    }
    file.close();
}

void ghiketqua(const string& name, const vector<int>& path, int distance, ofstream& out) {
    out << "=== " << name << " ===\n";
    out << "Chuoi da xu ly la: ";
    for (size_t i = 0; i < path.size(); ++i) {
        out << path[i];
        if (i != path.size() - 1) out << " -> ";
    }
    out << "\nTong khoang cach dau doc di chuyen: " << distance << "\n\n";
}

int tinh_khoang_cach(const vector<int>& path) {
    int sum = 0;
    for (size_t i = 1; i < path.size(); ++i)
        sum += abs(path[i] - path[i - 1]);
    return sum;
}

void FIFO(ofstream& out) {
    vector<int> path = { head };
    path.insert(path.end(), requests.begin(), requests.end());
    ghiketqua("FIFO", path, tinh_khoang_cach(path), out);
}

void SSTF(ofstream& out) {
    vector<int> path = { head };
    vector<int> rq = requests;
    int cur = head;
    while (!rq.empty()) {
        auto it = min_element(rq.begin(), rq.end(), [cur](int a, int b) {
            return abs(cur - a) < abs(cur - b);
            });
        cur = *it;
        path.push_back(cur);
        rq.erase(it);
    }
    ghiketqua("SSTF", path, tinh_khoang_cach(path), out);
}

void SCAN(ofstream& out, bool toLeft, int maxCylinder) {
    vector<int> rq = requests, path = { head };
    sort(rq.begin(), rq.end());

    vector<int> left, right;
    for (int r : rq) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }

    if (toLeft) {
        reverse(left.begin(), left.end());
        path.insert(path.end(), left.begin(), left.end());
        path.push_back(0);
        path.insert(path.end(), right.begin(), right.end());
        ghiketqua("SCAN (left)", path, tinh_khoang_cach(path), out);
    }
    else {
        path.insert(path.end(), right.begin(), right.end());
        path.push_back(maxCylinder);
        reverse(left.begin(), left.end());
        path.insert(path.end(), left.begin(), left.end());
        ghiketqua("SCAN (right)", path, tinh_khoang_cach(path), out);
    }
}

void CSCAN(ofstream& out, bool toLeft, int maxCylinder) {
    vector<int> rq = requests, path = { head };
    sort(rq.begin(), rq.end());

    vector<int> left, right;
    for (int r : rq) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }

    if (toLeft) {
        reverse(left.begin(), left.end());
        path.insert(path.end(), left.begin(), left.end());
        path.push_back(0);
        path.push_back(maxCylinder);
        reverse(right.begin(), right.end());
        path.insert(path.end(), right.begin(), right.end());
        ghiketqua("C-SCAN (left)", path, tinh_khoang_cach(path), out);
    }
    else {
        path.insert(path.end(), right.begin(), right.end());
        path.push_back(maxCylinder);
        path.push_back(0);
        path.insert(path.end(), left.begin(), left.end());
        ghiketqua("C-SCAN (right)", path, tinh_khoang_cach(path), out);
    }
}

void LOOK(ofstream& out, bool toLeft) {
    vector<int> rq = requests, path = { head };
    sort(rq.begin(), rq.end());

    vector<int> left, right;
    for (int r : rq) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }

    reverse(left.begin(), left.end());

    if (toLeft) {
        path.insert(path.end(), left.begin(), left.end());
        path.insert(path.end(), right.begin(), right.end());
        ghiketqua("LOOK (left)", path, tinh_khoang_cach(path), out);
    }
    else {
        path.insert(path.end(), right.begin(), right.end());
        path.insert(path.end(), left.begin(), left.end());
        ghiketqua("LOOK (right)", path, tinh_khoang_cach(path), out);
    }
}

void CLOOK(ofstream& out, bool toLeft) {
    vector<int> rq = requests, path = { head };
    sort(rq.begin(), rq.end());

    vector<int> left, right;
    for (int r : rq) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }

    if (toLeft) {
        reverse(left.begin(), left.end());
        path.insert(path.end(), left.begin(), left.end());
        path.insert(path.end(), right.begin(), right.end());
        ghiketqua("C-LOOK (left)", path, tinh_khoang_cach(path), out);
    }
    else {
        path.insert(path.end(), right.begin(), right.end());
        path.insert(path.end(), left.begin(), left.end());
        ghiketqua("C-LOOK (right)", path, tinh_khoang_cach(path), out);
    }
}

int main() {
    docdulieu("input.txt");
    ofstream out("output.txt");
    if (!out) {
        cerr << "Khong the mo file output!\n";
        return 1;
    }

    out << "Vi tri dau doc ban dau: " << head << "\n";
    out << "Danh sach yeu cau: ";
    for (int r : requests) out << r << " ";
    out << "\n\n";

    int chon, huong = 0, maxCylinder = 199;
    cout << "Chon thuat toan:\n";
    cout << "1. FIFO\n2. SSTF\n3. SCAN\n4. C-SCAN\n5. LOOK\n6. C-LOOK\nNhap lua chon (1-6): ";
    cin >> chon;

    if (chon >= 3 && chon <= 6) {
        cout << "Chon huong (0 = Trai, 1 = Phai): ";
        cin >> huong;
        cout << "Nhap gia tri bien bia (vi du 199): ";
        cin >> maxCylinder;
    }

    switch (chon) {
    case 1: FIFO(out); break;
    case 2: SSTF(out); break;
    case 3: SCAN(out, huong == 0, maxCylinder); break;
    case 4: CSCAN(out, huong == 0, maxCylinder); break;
    case 5: LOOK(out, huong == 0); break;
    case 6: CLOOK(out, huong == 0); break;
    default: cout << "Lua chon khong hop le!\n"; break;
    }

    out.close();
    cout << "Da ghi ket qua vao file output.txt\n";
    return 0;
}
