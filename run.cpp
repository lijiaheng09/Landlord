#include <bits/stdc++.h>

#include "jsoncpp/json.h"

using namespace std;

inline void read(string file, Json::Value &v)
{
	static Json::Reader reader;
	static ifstream fin;
	string str;
	fin.open(file);
	getline(fin, str);
	reader.parse(str, v);
	fin.close();
}

inline void write(string file, const Json::Value &v)
{
	static Json::FastWriter writer;
	static ofstream fout;
	fout.open(file);
	fout << writer.write(v);
	fout.close();
}

const string NAME[3] = {"0", "1", "2"};
string PLAYER[3] = {"sample", "sample", "sample"};
Json::Value dat = Json::objectValue;
Json::Value &data_log = dat["log"] = Json::arrayValue;
Json::Value input[3] = {Json::objectValue, Json::objectValue, Json::objectValue};
Json::Value *requests[3] = {
	&(input[0]["requests"] = Json::arrayValue),
	&(input[1]["requests"] = Json::arrayValue),
	&(input[2]["requests"] = Json::arrayValue)
};
Json::Value *responses[3] = {
	&(input[0]["responses"] = Json::arrayValue),
	&(input[1]["responses"] = Json::arrayValue),
	&(input[2]["responses"] = Json::arrayValue)
};

void init()
{
	ifstream fin("config.txt");
	fin >> PLAYER[0] >> PLAYER[1] >> PLAYER[2];
}

int call_judger()
{
	int nxt = 0;
	write("data.json", dat);
	system("python3 judger.py <data.json >output.json");
	Json::Value &t = data_log[data_log.size()];
	read("output.json", t);
	if (t.isMember("initdata"))
		dat["initdata"] = t["initdata"];
	data_log[data_log.size() - 1]["output"] = t;
	if (t["command"] == "request")
	{
		for (int i = 0; i < 3; i++)
			if (t["content"].isMember(NAME[i])) {
				requests[i]->append(t["content"][NAME[i]]);
				nxt = i;
			}
		if(t.isMember("display") && t["display"].isMember("errored")) {
			for (int p = 0; p < 3; p++)
				if (t["display"]["errored"][p] != Json::nullValue) {
					write("data.json", dat);
					exit(0);
				}
		}
	}
	else {
		printf(
			"\nPlayer 0: %d\nPlayer 1: %d\nPlayer 2: %d\n",
			t["content"]["0"].asInt(),
			t["content"]["1"].asInt(),
			t["content"]["2"].asInt()
		);
		write("data.json", dat);
		exit(0);
	}
	return nxt;
}

void call_player(int i)
{
	static char Cmd[300];
	write("input.json", input[i]);
	sprintf(Cmd, "./%s <input.json >output.json", PLAYER[i].c_str());
	if (system(Cmd))
		exit(0);
	Json::Value &t = data_log[data_log.size()][NAME[i]];
	read("output.json", t);
	t["verdict"] = "OK";
	responses[i]->append(t["response"]);
}

int main()
{
	init();
	call_judger();
	// getchar();
	int nxt = 0, num = 0;
	while (1)
	{
		call_player(nxt);
		printf("\r#%d: Player %d", ++num, nxt);
		fflush(stdout);
		// getchar();
		nxt = call_judger();
		// getchar();
	}
	return 0;
}
