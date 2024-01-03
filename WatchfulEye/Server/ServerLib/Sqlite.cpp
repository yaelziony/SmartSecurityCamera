#include "Sqlite.h"

using namespace std;
using namespace cv;

Sqlite::Sqlite() {
	init();
}
void Sqlite::init() {
	//Initialize the static variables.are used to get the configurations
	ConfigurationServer configurationServer;
	path = configurationServer.getSqliteMembers();
}
int Sqlite::createTable(string path) {
	//Create a connection to the database.
	sqlite3* db;
	int rc = sqlite3_open(path.c_str(), &db);
	int isValid = IsValid(rc, db, "Error opening database: ");

	// Create the table.
	const char* sql = "CREATE TABLE detections (id INTEGER PRIMARY KEY, width FLOAT, height FLOAT, top FLOAT, left FLOAT, time TEXT, AvgR FLOAT, AvgG FLOAT, AvgB FLOAT);";
	rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
	isValid = IsValid(rc, db, "Error creating table: ");

	// Close the connection to the database.
	sqlite3_close(db);

	cout << "Table created successfully." << endl;
	return 0;
}

int Sqlite::insertData(string path, Mat& image, Rect originalBox, string time)
{
	Rect box = NormalizeBox(originalBox, image);
	Avgs avgs = calcAvgs(image, box);

	sqlite3* db;
	int rc = sqlite3_open(path.c_str(), &db);

	std::string sql = "INSERT INTO detections (width, height, top, left, time, AvgR, AvgG, AvgB) VALUES (" +
		std::to_string(box.width) + ", " +
		std::to_string(box.height) + ", " +
		std::to_string(box.y) + ", " +
		std::to_string(box.x) + ", '" +
		time + "', " +
		std::to_string(avgs.AvgR) + ", " +
		std::to_string(avgs.AvgG) + ", " +
		std::to_string(avgs.AvgB) + ");";

	rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
	int isValid = IsValid(rc, db, "Error opening database: ");

	// Close the connection to the database.
	sqlite3_close(db);
	cout << "Data inserted successfuly" << endl;
	return 0;
}

Rect Sqlite::NormalizeBox(const Rect& originalBox, cv::Mat& image) {
	Rect box = originalBox;
	if (box.x < 0)
		box.x = 0;
	if (box.y < 0)
		box.y = 0;
	if (box.height < 0)
		box.height = 0;
	if (box.width < 0)
		box.width = 0;
	if (box.x + box.width > image.cols)
		box.width = image.cols - box.x;
	if (box.y + box.height > image.rows)
		box.height = image.rows - box.y;
	return box;
}

//per detetction - calculate avgs of each channel of the 'sub image'
Avgs Sqlite::calcAvgs(cv::Mat& image, Rect box) {
	Avgs avgs;
	int sumR = 0, sumG = 0, sumB = 0;
	for (int i = box.y; i < box.y + box.height; i++) {
		for (int j = box.x; j < box.x + box.width; j++) {
			sumB += image.at<Vec3b>(i, j)[RChannel];
			sumG += image.at<Vec3b>(i, j)[GChannel];
			sumR += image.at<Vec3b>(i, j)[BChannel];
		}
	}
	int area = (box.height) * (box.width);
	avgs.AvgB = sumB / area;
	avgs.AvgG = sumG / area;
	avgs.AvgR = sumR / area;
	return avgs;
}

int Sqlite::IsValid(int rc, sqlite3* db, string message) {
	if (rc != SQLITE_OK) {
		cout << message << sqlite3_errmsg(db) << endl;
		Logging::writeTolog(spdlog::level::info, message + " " + sqlite3_errmsg(db));
		return 1;
	}
	return 0;
}