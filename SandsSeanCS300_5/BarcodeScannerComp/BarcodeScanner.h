#pragma once

template <class D>
class BarcodeScanner
{
private:
	typename D::key_type kt;
	typename D::value_type vt;

	D *database;

public:
	BarcodeScanner(D* db) :database(db) {}
	~BarcodeScanner() { if (database) { delete database; database = nullptr; } }


	void scan(const kt &k, const vt &v) { database->insert(k, v); }
	const vt &scan(const kt &k) const { return database->find(k); }
};