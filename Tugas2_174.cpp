#include <iostream>
#include <string>
using namespace std;
//stack BST
struct Node
{
	string nopol;
	string pemilik;
	string status;
	
	Node *left;
	Node *right;
};
Node *root = NULL;
//stack
struct Stack
{
	string aksi;
	string nopol;
};
Stack stackUndo[100];
int top = -1;
//queue
string queueParkir[100];
int depan = 0;
int belakang = -1;

//kapasitas parkir
int kapasitas = 3;
int jumlahParkir = 0;

//function stack
bool stackKosong()
{
	return top == -1;
}

void push(string aksi, string nopol)
{
	top++;
	
	stackUndo[top].aksi = aksi;
	stackUndo[top].nopol = nopol;
}

void pop(string &aksi, string &nopol)
{
	aksi = stackUndo[top].aksi;
	nopol = stackUndo[top].nopol;
	
	top--;
}

//function queue
bool queueKosong()
{
	return belakang == -1;
}

void enqueue(string nopol)
{
	belakang++;
	queueParkir[belakang]=nopol;
}

string dequeue()
{
	string data = queueParkir[depan];
	
	for (int i=0; i<belakang; i++)
	{
		queueParkir[i]= queueParkir[i+1];
	}
	belakang--;
	return data;
}

//function bst
Node* buatNode(string nopol, string pemilik)
{
	Node *baru = new Node;
	
	baru->nopol = nopol;
	baru->pemilik = pemilik;
	baru->status = "Di Luar Parkiran";
	
	baru->left = NULL;
	baru->right = NULL;
	
	return baru;
}

Node* insert(Node *root, string nopol, string pemilik)
{
	if (root == NULL)
	{
		return buatNode(nopol, pemilik);
	}
	
	if (nopol < root->nopol)
	{
		root->left = insert(root->left, nopol, pemilik);
	}
	
	else if (nopol > root->nopol)
	{
		root->right = insert(root->right, nopol, pemilik);
	}
	
	return root;
}

Node* search(Node *root, string nopol)
{
	if (root == NULL || root->nopol == nopol)
	{
		return root;
	}
	
	if (nopol < root->nopol)
	{
		return search(root->left, nopol);
	}
	else {
		return search(root->right, nopol);
	}
}

void inorder(Node *root)
{
	if (root != NULL)
	{
		inorder(root->left);
		
		cout << "Nomor Polisi: " << root->nopol << endl;
		cout << "Nama Pemilik: " << root->pemilik << endl;
		cout << "Status         : " << root->status << endl;
		cout << "===============================" << endl;
		
		inorder(root->right);
	}
}

Node* minValue(Node *node)
{
	Node *current = node;
	
	while (current && current->left != NULL)
	{
		current = current->left;
	}
	
	return current;
}

Node* hapus(Node *root, string nopol)
{
	if (root == NULL)
	{
		return root;
	}
	
	if (nopol < root->nopol)
	{
		root->left = hapus(root->left, nopol);
	}
	
	else if (nopol > root->nopol)
	{
		root->right = hapus(root->right, nopol);
	}
	
	else
	{
		if (root->left == NULL) 
		{
            Node *temp = root->right;
            delete root;
            return temp;
        }

        else if (root->right == NULL) 
        {
            Node *temp = root->left;
            delete root;
            return temp;
        }

        Node *temp = minValue(root->right);

        root->nopol = temp->nopol;
        root->pemilik = temp->pemilik;
        root->status = temp->status;

        root->right = hapus(root->right, temp->nopol);
	}
	return root;
}

//menu function
void tambahKendaraan()
{
	string nopol;
    string pemilik;

    cin.ignore();

    cout << "Masukkan Nomor Polisi : ";
    getline(cin, nopol);

    cout << "Masukkan Nama Pemilik : ";
    getline(cin, pemilik);

    if (search(root, nopol) != NULL) 
    {
        cout << "Data kendaraan sudah ada!" << endl;
        return;
    }

    root = insert(root, nopol, pemilik);

    cout << "Data kendaraan berhasil ditambahkan" << endl;
}

void tampilKendaraan()
{
	if (root == NULL)
	{
		cout << "Data kosong!!" << endl;
		return;
	}
	inorder(root);
}

void kendaraanMasuk()
{
	string nopol;
    cin.ignore();

    cout << "Masukkan Nomor Polisi : ";
    getline(cin, nopol);

    Node *kendaraan = search(root, nopol);

    if (kendaraan == NULL) 
    {
        cout << "Kendaraan tidak ditemukan!" << endl;
        return;
    }

    if (kendaraan->status == "Di Parkiran") 
    {
        cout << "Kendaraan sudah berada di parkiran!" << endl;
        return;
    }

    if (jumlahParkir < kapasitas) 
    {
        kendaraan->status = "Di Parkiran";
        jumlahParkir++;

        push("masuk", nopol);

        cout << "Kendaraan berhasil masuk" << endl;
    }

    else 
    {
        enqueue(nopol);

        cout << "Parkiran penuh, kendaraan masuk ke dalam antrian" << endl;
    }
}

void kendaraanKeluar()
{
	string nopol;
    cin.ignore();

    cout << "Masukkan Nomor Polisi : ";
    getline(cin, nopol);

    Node *kendaraan = search(root, nopol);

    if (kendaraan == NULL) 
    {
        cout << "Kendaraan tidak ditemukan!" << endl;
        return;
    }

    if (kendaraan->status == "Di Luar Parkiran") 
    {
        cout << "Kendaraan sedang tidak di parkiran!" << endl;
        return;
    }

    kendaraan->status = "Di Luar Parkiran";

    jumlahParkir--;

    push("keluar", nopol);

    cout << "Kendaraan berhasil keluar" << endl;

    if (!queueKosong()) 
    {
        string next = dequeue();
        
        Node *data = search(root, next);

        if (data != NULL) 
        {
            data->status = "Di Parkiran";

            jumlahParkir++;
        }

        cout << "Kendaraan berikutnya langsung masuk dari antrian" << endl;
    }
}

void hapusKendaraan()
{
	string nopol;
    cin.ignore();

    cout << "Masukkan Nomor Polisi : ";
    getline(cin, nopol);

    if (search(root, nopol) == NULL) 
    {
        cout << "Kendaraan tidak ditemukan!" << endl;
        return;
    }
    root = hapus(root, nopol);

    cout << "Data kendaraan berhasil dihapus" << endl;
}

void undo()
{
	if (stackKosong()) 
	{
        cout << "Tidak ada aksi untuk di-undo" << endl;
        return;
    }

    string aksi;
    string nopol;

    pop(aksi, nopol);

    Node *kendaraan = search(root, nopol);

    if (kendaraan == NULL) 
    {
        return;
    }

    if (aksi == "masuk") 
    {

        kendaraan->status = "Di Luar Parkiran";

        jumlahParkir--;

        cout << "Undo: Kendaraan keluar dari parkiran" << endl;
    }

    else if (aksi == "keluar") 
    {
        kendaraan->status = "Di Parkiran";
        jumlahParkir++;

        cout << "Undo: Kendaraan masuk kembali ke parkiran" << endl;
    }
}

void tampilAntrian()
{
	if (queueKosong()) 
	{
        cout << "Antrian kosong" << endl;
        return;
    }

    for (int i = depan; i <= belakang; i++) 
    {
        cout << queueParkir[i]
             << " menunggu giliran masuk parkiran" << endl;
    }
}

//main
int main()
{
	int pilih;

do {
	cout << endl;
    cout << "==== SISTEM PARKIR BANDARA GSA ====" << endl;
    cout << "1. Tambah Data Kendaraan" << endl;
    cout << "2. Tampilkan Kendaraan" << endl;
    cout << "3. Kendaraan Masuk" << endl;
    cout << "4. Kendaraan Keluar" << endl;
    cout << "5. Hapus Data Kendaraan" << endl;
    cout << "6. Undo Aksi Terakhir" << endl;
    cout << "7. Tampilkan Antrian" << endl;
    cout << "0. Keluar" << endl;
    cout << "Pilih Menu : ";
    cin >> pilih;
    
    switch (pilih)
    {
		case 1:
		tambahKendaraan();
		break;
		
		case 2:
		tampilKendaraan();
		break;
		
		case 3:
		kendaraanMasuk();
		break;
		
		case 4:
		kendaraanKeluar();
		break;
		
		case 5:
		hapusKendaraan();
		break;
		
		case 6:
		undo();
		break;
		
		case 7:
		tampilAntrian();
		break;
		
		case 0:
		cout << "Program selesai.." << endl;
		break;
		
		default:
		cout << "Menu tidak tersedia" << endl;
			
		
		}
	}while (pilih != 0);
	
	return 0;
}

