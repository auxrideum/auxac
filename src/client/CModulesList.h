class CModulesList {
public:
	CModulesList();
	~CModulesList();

	void Update();

	vector<string> Get() { return modulesList; };

private:
	vector<string> modulesList;
};

extern CModulesList* pModulesList;