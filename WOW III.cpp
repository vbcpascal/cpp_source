#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

int sum_elements;
int sum_health[5];
int sum_force[5];
int sum_k;
int sum_city;
int sum_time;
int timeID = 0;
bool gameend = false;
string redStr = "red";
string blueStr = "blue";

void coutTime(int t = timeID) {
	cout << setfill('0') << setw(3) << t << ":";
}

//dragon ninja iceman lion wolf
//sword bomb arrow

class Headquarter;
class Warrior;
class City;

City* cities[30];

class Weapon {
protected:
	int weaID;
	Warrior* weaUser;
public:
	int weaForce;
	bool weaGG;
	bool weaUsed;
	friend class Warrior;
	Weapon(Warrior* _user, int _force, int _id);
	virtual void userChange(Warrior* w) = 0;
	virtual bool attack(Warrior* user, Warrior* recer);
};
class pSword :public Weapon {
public:
	pSword(Warrior* _user);
	void userChange(Warrior* w);
};
class pBomb :public Weapon {
public:
	pBomb(Warrior* _user);
	void userChange(Warrior* w);
	bool attack(Warrior* user, Warrior* recer);
};
class pArrow :public Weapon {
public:
	pArrow(Warrior* _user);
	void userChange(Warrior* w);
	bool attack(Warrior* user, Warrior* recer);
};

bool mycomp(Weapon* a, Weapon *b) {
	return (a->weaUsed < b->weaUsed);
}

//dragon ninja iceman lion wolf
class Warrior{
protected:
	int warID;
	int warForce;
	int weaponIndex;
	City* warCity;
	Weapon* warWeapon[3][10];
	Headquarter* warHead;
	void actMakeWeapon(int m);

public:
	friend class wWolf;
	friend class Weapon;
	friend class City;
	string warName;
	int warHealth;
	int warWeaNum;
	int warWeaNumEach[3];
	bool nRun;
	bool nYell;

	Warrior(Headquarter* _head, City* _c, int _ID, int _health, int _force);
	//~Warrior();
	int getWarriorForce() { return warForce; }
	int actAttack(Warrior* enemy);

	void actSort();
	void actWin(Warrior* enemy);

	virtual void prtMake();
	virtual bool actRun();
	virtual void actGo();
	virtual void actRob(Warrior* enemy) {}
	
	bool cannotAct();
};
class wDragon :public Warrior {
public:
	wDragon(Headquarter* _head, City* _c, int _ID, int _health, int _force);
};
class wNinja :public Warrior {
public:
	wNinja(Headquarter* _head, City* _c, int _ID, int _health, int _force);
};
class wIceman :public Warrior {
public:
	wIceman(Headquarter* _head, City* _c, int _ID, int _health, int _force);
	void actGo();
};
class wLion :public Warrior {
private:
	int nLoyalty;
	int nk;
public:
	wLion(Headquarter* _head, City* _c, int _ID, int _health, int _force, int k);
	void prtMake();
	bool actRun();
	void actGo();
};
class wWolf :public Warrior {
private:
	int nLoyalty;
public:
	wWolf(Headquarter* _head, City* _c, int _ID, int _health, int _force);
	virtual void actRob(Warrior* enemy);
};

class City {
	Warrior* blueWarrior;
	Warrior* redWarrior;
public:
	int cityID;
	City(int _id) :cityID(_id), blueWarrior(NULL), redWarrior(NULL) {}
	void changeWarRed(Warrior* w) { redWarrior = w; }
	void changeWarBlue(Warrior* w) { blueWarrior = w; }
	void prtWar();
	void actWolfRob();
	void actGameCtrl();
	void actYell();
	void prtWeaponNum();
	int actGame(Warrior* (&a), Warrior* (&b));
};

class Headquarter {
private:
	bool headCanMake;
	string headName;
	int elements;
	int headOrder[5];
	int orderIndex;
	int headWarID;
	int headWarHealth[5];
	int headWarForce[5];
	int headLionK;
	Warrior* headWarrior[30];
public:
	Headquarter(int _n, int _ele, int _order[], int _h[], int _f[], int _k);
	//~Headquarter();
	void actHeadMake(City* c);
	void actWarGo();

	bool actLionRun();
	
	int getHeadEle() { return elements; }
	string getHeadName() { return headName; }
	
	void prtEle();
};

//************************************************* WEAPON *************************************************//  

Weapon::Weapon(Warrior* _user, int _force, int _id) :weaUser(_user), weaForce(_force), weaID(_id) {
	weaUsed = false;
	weaGG = false;
}

pSword::pSword(Warrior* _user) :Weapon(_user, _user->getWarriorForce() / 5, 0) { }
pBomb::pBomb(Warrior* _user) :Weapon(_user, _user->getWarriorForce() * 2 / 5, 1) { }
pArrow::pArrow(Warrior* _user) :Weapon(_user, _user->getWarriorForce() * 3 / 10, 2) { }

void pSword::userChange(Warrior* w) { weaUser = w; weaForce = w->getWarriorForce() / 5; }
void pBomb::userChange(Warrior* w) { weaUser = w; weaForce = w->getWarriorForce() * 2 / 5; }
void pArrow::userChange(Warrior* w) { weaUser = w; weaForce = w->getWarriorForce() * 3 / 10; }

bool Weapon::attack(Warrior* user, Warrior* recer) {
	recer->warHealth -= weaForce;
	return false;
}
bool pBomb::attack(Warrior* user, Warrior* recer) {
	recer->warHealth -= weaForce;
	if (user->warName != "ninja") user->warHealth -= weaForce / 2;
	weaGG = true;
	if (weaGG) {
		user->warWeaNum--;
		user->warWeaNumEach[1]--;
	}
	return true;
}
bool pArrow::attack(Warrior* user, Warrior* recer) {
	recer->warHealth -= weaForce;
	if (weaUsed) weaGG = true;
	else weaUsed = true;
	if (weaGG) {
		user->warWeaNum--;
		user->warWeaNumEach[2]--;
	}
	return weaGG;
}

//************************************************* WARRIOR *************************************************//  

Warrior::Warrior(Headquarter* _head, City* _c, int _ID, int _health, int _force) :
	warID(_ID), warHealth(_health), warHead(_head), warForce(_force), warCity(_c) {
	nRun = false; nYell = false;
}
wDragon::wDragon(Headquarter* _head, City* _c, int _ID, int _health, int _force) : Warrior(_head, _c, _ID, _health, _force) {
	actMakeWeapon(warID % 3);
	warName = "dragon";
}
wNinja::wNinja(Headquarter* _head, City* _c, int _ID, int _health, int _force) : Warrior(_head, _c, _ID, _health, _force) {
	actMakeWeapon(warID % 3);
	actMakeWeapon((warID + 1) % 3);
	warName = "ninja";
}
wIceman::wIceman(Headquarter* _head, City* _c, int _ID, int _health, int _force) : Warrior(_head, _c, _ID, _health, _force) {
	actMakeWeapon(warID % 3);
	warName = "iceman";
}
wLion::wLion(Headquarter* _head, City* _c, int _ID, int _health, int _force, int k) : Warrior(_head, _c, _ID, _health, _force), nk(k) {
	actMakeWeapon(warID % 3);
	nLoyalty = warHead->getHeadEle();
	warName = "lion";
}
wWolf::wWolf(Headquarter* _head, City* _c, int _ID, int _health, int _force) : Warrior(_head, _c, _ID, _health, _force) {
	warName = "wolf";
}
/*Warrior::~Warrior() {
	for (int i = 0; i < warWeaNumEach[0]; i++)delete warWeapon[0][i];
	for (int i = 0; i < warWeaNumEach[1]; i++)delete warWeapon[1][i];
	for (int i = 0; i < warWeaNumEach[2]; i++)delete warWeapon[2][i];
}*/

void Warrior::actMakeWeapon(int m) {
	switch (m) {
	case 0:
		warWeapon[0][warWeaNumEach[0]] = new pSword(this);
		warWeaNum++;
		warWeaNumEach[0]++;
		break;
	case 1:
		warWeapon[1][warWeaNumEach[1]] = new pBomb(this);
		warWeaNum++;
		warWeaNumEach[1]++;
		break;
	case 2:
		warWeapon[2][warWeaNumEach[2]] = new pArrow(this);
		warWeaNum++;
		warWeaNumEach[2]++;
		break;
	}
}
void Warrior::prtMake() {
	coutTime(timeID);
	cout << "00 " << warHead->getHeadName() << " " << warName << " " << warID << " born" << endl;
}
void Warrior::actGo() {
	if (warHead->getHeadName() == redStr) {
		warCity->changeWarRed(NULL);
		warCity = cities[warCity->cityID + 1];
		warCity->changeWarRed(this);
	}
	else {
		warCity->changeWarBlue(NULL);
		warCity = cities[warCity->cityID - 1];
		warCity->changeWarBlue(this);
	}
}
void Warrior::actSort() {
	if (warWeaNumEach[2]) sort(warWeapon[2], warWeapon[2] + warWeaNumEach[2], mycomp);
	if (warWeaNumEach[0]) weaponIndex = 0;
	else if (warWeaNumEach[1]) weaponIndex = 10;
	else if (warWeaNumEach[2]) weaponIndex = 20;
}
void Warrior::actWin(Warrior* enemy) {
	enemy->actSort();
	if (enemy->warWeaNumEach[0] != 0) {
		while (warWeaNum < 10 && enemy->warWeaNumEach[0]) {
			warWeapon[0][warWeaNumEach[0]] = enemy->warWeapon[0][enemy->warWeaNumEach[0] - 1];
			enemy->warWeapon[0][enemy->warWeaNumEach[0] - 1] = NULL;
			warWeapon[0][warWeaNumEach[0]]->userChange(this);
			enemy->warWeaNumEach[0]--; enemy->warWeaNum--;
			warWeaNumEach[0]++; warWeaNum++;
		}
	}
	if (enemy->warWeaNumEach[1] != 0) {
		while (warWeaNum < 10 && enemy->warWeaNumEach[1]) {
			warWeapon[1][warWeaNumEach[1]] = enemy->warWeapon[1][enemy->warWeaNumEach[1] - 1];
			enemy->warWeapon[1][enemy->warWeaNumEach[1] - 1] = NULL;
			warWeapon[1][warWeaNumEach[1]]->userChange(this);
			enemy->warWeaNumEach[1]--; enemy->warWeaNum--;
			warWeaNumEach[1]++; warWeaNum++;
		}
	}
	if (enemy->warWeaNumEach[2] != 0) {
		while (warWeaNum < 10 && enemy->warWeaNumEach[2]) {
			warWeapon[2][warWeaNumEach[2]] = enemy->warWeapon[2][enemy->warWeaNumEach[2] - 1];
			enemy->warWeapon[2][enemy->warWeaNumEach[2] - 1] = NULL;
			warWeapon[2][warWeaNumEach[2]]->userChange(this);
			enemy->warWeaNumEach[2]--; enemy->warWeaNum--;
			warWeaNumEach[2]++; warWeaNum++;
		}
		//coutTime();
		//cout << "35 " << warHead->getHeadName() << " wolf " << warID << " took " << x << " arrow from" <<
		//	enemy->warHead->getHeadName() << " " << enemy->warName << " " << enemy->warID << " in city " << warCity->cityID << endl;
	}
	for (int i = 0; i < warWeaNumEach[0]; i++) if (warWeapon[0][i]->weaUser == enemy) delete warWeapon[0][i];
	for (int i = 1; i < warWeaNumEach[1]; i++) if (warWeapon[1][i]->weaUser == enemy) delete warWeapon[1][i];
	for (int i = 2; i < warWeaNumEach[2]; i++) if (warWeapon[2][i]->weaUser == enemy) delete warWeapon[2][i];
	actSort();
}
void wIceman::actGo() {
	warHealth -= warHealth / 10;
	if (warHealth <= 0) {
		nRun = true;
		if (warHead->getHeadName() == redStr) warCity->changeWarRed(NULL);
		else warCity->changeWarBlue(NULL);
	}
	if (warHead->getHeadName() == redStr) {
		warCity->changeWarRed(NULL);
		warCity = cities[warCity->cityID + 1];
		warCity->changeWarRed(this);
	}
	else {
		warCity->changeWarBlue(NULL);
		warCity = cities[warCity->cityID - 1];
		warCity->changeWarBlue(this);
	}
}
void wLion::prtMake() {
	coutTime(timeID);
	cout << "00 " << warHead->getHeadName() << " lion " << warID << " born" << endl;
	cout << "Its loyalty is " << nLoyalty << endl;
}
void wLion::actGo() {
	nLoyalty -= nk;
	if (warHead->getHeadName() == redStr) {
		warCity->changeWarRed(NULL);
		warCity = cities[warCity->cityID + 1];
		warCity->changeWarRed(this);
	}
	else {
		warCity->changeWarBlue(NULL);
		warCity = cities[warCity->cityID - 1];
		warCity->changeWarBlue(this);
	}
}
void wWolf::actRob(Warrior* enemy) {
	//000:35 blue wolf 2 took 3 bomb from red dragon 2 in city 4
	int x = 0;
	if (enemy->warWeaNumEach[0] != 0) {
		while (warWeaNum < 10 && enemy->warWeaNumEach[0]) {
			x++;
			warWeapon[0][warWeaNumEach[0]] = enemy->warWeapon[0][enemy->warWeaNumEach[0] - 1];
			enemy->warWeapon[0][enemy->warWeaNumEach[0] - 1] = NULL;
			warWeapon[0][warWeaNumEach[0]]->userChange(this);
			enemy->warWeaNumEach[0]--; enemy->warWeaNum--;
			warWeaNumEach[0]++; warWeaNum++;
		}
		coutTime();
		cout << "35 " << warHead->getHeadName() << " wolf " << warID << " took " << x << " sword from " <<
			enemy->warHead->getHeadName() << " " << enemy->warName << " " << enemy->warID << " in city " << warCity->cityID << endl;
		return;
	}
	if (enemy->warWeaNumEach[1] != 0) {
		while (warWeaNum < 10 && enemy->warWeaNumEach[1]) {
			x++;
			warWeapon[1][warWeaNumEach[1]] = enemy->warWeapon[1][enemy->warWeaNumEach[1] - 1];
			enemy->warWeapon[1][enemy->warWeaNumEach[1] - 1] = NULL;
			warWeapon[1][warWeaNumEach[1]]->userChange(this);
			enemy->warWeaNumEach[1]--; enemy->warWeaNum--;
			warWeaNumEach[1]++; warWeaNum++;
		}
		coutTime();
		cout << "35 " << warHead->getHeadName() << " wolf " << warID << " took " << x << " bomb from " <<
			enemy->warHead->getHeadName() << " " << enemy->warName << " " << enemy->warID << " in city " << warCity->cityID << endl;
		return;
	}
	if (enemy->warWeaNumEach[2] != 0) {
		while (warWeaNum < 10 && enemy->warWeaNumEach[2]) {
			x++;
			warWeapon[2][warWeaNumEach[2]] = enemy->warWeapon[2][enemy->warWeaNumEach[2] - 1];
			enemy->warWeapon[2][enemy->warWeaNumEach[2] - 1] = NULL;
			warWeapon[2][warWeaNumEach[2]]->userChange(this);
			enemy->warWeaNumEach[2]--; enemy->warWeaNum--;
			warWeaNumEach[2]++; warWeaNum++;
		}
		coutTime();
		cout << "35 " << warHead->getHeadName() << " wolf " << warID << " took " << x << " arrow from " <<
			enemy->warHead->getHeadName() << " " << enemy->warName << " " << enemy->warID << " in city " << warCity->cityID << endl;
		return;
	}
}

int Warrior::actAttack(Warrior* enemy) {  //0:alive , -1:attacker died , -2:enemy died , -3:died
	if (warWeaNum == 0) return 0;
	int wa = weaponIndex / 10, wb = weaponIndex % 10;
	Weapon* myWeapon = warWeapon[wa][wb];
	bool weapon_gg = myWeapon->attack(this, enemy);
	int tmp = 0;
	if (warHealth <= 0) tmp -= 1;
	if (enemy->warHealth <= 0)tmp -= 2;
	if (warWeaNum == 0)return tmp;

	if (weapon_gg) {
		delete myWeapon;
		for (int i = wb; i < warWeaNumEach[wa]; i++)
			warWeapon[wa][i] = warWeapon[wa][i + 1];
		warWeapon[wa][warWeaNumEach[wa]] = NULL;
	}
	else wb++;
	while (wb >= warWeaNumEach[wa]) {
		wa = (wa + 1) % 3;
		wb = 0;
	}
	weaponIndex = wa * 10 + wb;
	return tmp;
}
bool Warrior::actRun(){
	return false;
}
bool Warrior::cannotAct() {
	if (warWeaNum == 0) return true;
	if (warWeaNumEach[1] == 0 && warWeaNumEach[2] == 0 && warWeapon[0][0]->weaForce == 0) return true;
	return false;
}
bool wLion::actRun() {
	if (nLoyalty <= 0) {
		nRun = true;
		coutTime();
		cout << "05 " << warHead->getHeadName() << " lion " << warID << " ran away" << endl;
		if (warHead->getHeadName() == redStr) warCity->changeWarRed(NULL);
		else warCity->changeWarBlue(NULL);
	}
	return nRun;
}

//************************************************* CITY *************************************************//  

void City::prtWar() {
	//000:10 blue lion 1 marched to city 1 with 10 elements and force 5
	if (cityID == 0 && blueWarrior) {
		coutTime();
		cout << "10 blue " << blueWarrior->warName << " " << blueWarrior->warID << " reached red headquarter with "
			<< blueWarrior->warHealth << " elements and force " << blueWarrior->warForce << endl;
		coutTime();
		cout << "10 red headquarter was taken" << endl;
		gameend = true;
	}
	if ((cityID > 0) && (cityID <= sum_city) && redWarrior) {
		coutTime();
		cout << "10 red " << redWarrior->warName << " " << redWarrior->warID << " marched to city " << cityID <<
			" with " << redWarrior->warHealth << " elements and force " << redWarrior->warForce << endl;
	}
	if ((cityID > 0) && (cityID <= sum_city) && blueWarrior) {
		coutTime();
		cout << "10 blue " << blueWarrior->warName << " " << blueWarrior->warID << " marched to city " << cityID <<
			" with " << blueWarrior->warHealth << " elements and force " << blueWarrior->warForce << endl;
	}
	if (cityID == sum_city + 1 && redWarrior) {
		coutTime();
		cout << "10 red " << redWarrior->warName << " " << redWarrior->warID << " reached blue headquarter with "
			<< redWarrior->warHealth << " elements and force " << redWarrior->warForce << endl;
		coutTime();
		cout << "10 blue headquarter was taken" << endl;
		gameend = true;
	}
}
void City::actWolfRob() {
	if ((!blueWarrior) || (!redWarrior)) return;
	if ((blueWarrior->warName == "wolf") && (redWarrior->warName == "wolf"))return;
	if (blueWarrior->warName == "wolf") blueWarrior->actRob(redWarrior);
	if (redWarrior->warName == "wolf") redWarrior->actRob(blueWarrior);
}
void City::actGameCtrl() {
	if (blueWarrior) blueWarrior->actSort();
	if (redWarrior) redWarrior->actSort();
	if ((!blueWarrior) || (!redWarrior)) return;
	coutTime(); cout << "40 ";
	int tmp = 0;
	if (cityID % 2) {
		while (1) {
			tmp = actGame(redWarrior, blueWarrior);
			if (tmp)break;
			tmp = actGame(blueWarrior, redWarrior);
			if (tmp)break;
		}
	}
	else {
		while (1) {
			tmp = actGame(blueWarrior, redWarrior);
			if (tmp)break;
			tmp = actGame(redWarrior, blueWarrior);
			if (tmp)break;
		}
	}
}
void City::actYell() {
	//003:40 blue dragon 2 yelled in city 4
	if (redWarrior && redWarrior->nYell) {
		coutTime();
		cout << "40 red dragon " << redWarrior->warID << " yelled in city " << cityID << endl;
		redWarrior->nYell = false;
	}
	if (blueWarrior && blueWarrior->nYell) {
		coutTime();
		cout << "40 blue dragon " << blueWarrior->warID << " yelled in city " << cityID << endl;
		blueWarrior->nYell = false;
	}
}
void City::prtWeaponNum() {
	if (redWarrior) {
		coutTime();
		cout << "55 red " << redWarrior->warName << " " << redWarrior->warID << " has "
			<< redWarrior->warWeaNumEach[0] << " sword " << redWarrior->warWeaNumEach[1] << " bomb "
			<< redWarrior->warWeaNumEach[2] << " arrow and " << redWarrior->warHealth << " elements" << endl;
	}
	if (blueWarrior) {
		coutTime();
		cout << "55 blue " << blueWarrior->warName << " " << blueWarrior->warID << " has "
			<< blueWarrior->warWeaNumEach[0] << " sword " << blueWarrior->warWeaNumEach[1] << " bomb "
			<< blueWarrior->warWeaNumEach[2] << " arrow and " << blueWarrior->warHealth << " elements" << endl;
	}
}

int City::actGame(Warrior* (&a), Warrior* (&b)) {
	int tmp;
	tmp = a->actAttack(b);
	//000:40 red iceman 1 killed blue lion 12 in city 2 remaining 20 elements
	if (tmp == -1) {
		cout << b->warHead->getHeadName() << " " << b->warName << " " << b->warID << " killed " << a->warHead->getHeadName() << " "
			<< a->warName << " " << a->warID << " in city " << cityID << " remaining " << b->warHealth << " elements" << endl;
		a->nRun = true;
		b->actWin(a);
		if (b->warName == "dragon")b->nYell = true;
		actYell();
		a = NULL;
		return tmp;
	}
	else if (tmp == -2) {
		cout << a->warHead->getHeadName() << " " << a->warName << " " << a->warID << " killed " << b->warHead->getHeadName() << " "
			<< b->warName << " " << b->warID << " in city " << cityID << " remaining " << a->warHealth << " elements" << endl;
		b->nRun = true;
		a->actWin(b);
		if (a->warName == "dragon")a->nYell = true;
		actYell();
		b = NULL;
		return tmp;
	}
	else if (tmp == -3) {
		Warrior *c, *d;
		if (a->warHead->getHeadName() == redStr) {
			c = a; d = b;
		}
		else {
			c = b; d = a;
		}
		//both red iceman 1 and blue lion 12 died in city 2
		cout << "both red " << c->warName << " " << c->warID << " and blue " << d->warName << " " << d->warID
			<< " died in city " << cityID << endl;
		b->nRun = true;
		a->nRun = true;
		for (int i = 0; i < a->warWeaNumEach[0]; i++) if (a->warWeapon[0][i]) delete a->warWeapon[0][i];
		for (int i = 1; i < a->warWeaNumEach[1]; i++) if (a->warWeapon[1][i]) delete a->warWeapon[1][i];
		for (int i = 2; i < a->warWeaNumEach[2]; i++) if (a->warWeapon[2][i]) delete a->warWeapon[2][i];
		for (int i = 0; i < b->warWeaNumEach[0]; i++) if (b->warWeapon[0][i]) delete b->warWeapon[0][i];
		for (int i = 1; i < b->warWeaNumEach[1]; i++) if (b->warWeapon[1][i]) delete b->warWeapon[1][i];
		for (int i = 2; i < b->warWeaNumEach[2]; i++) if (b->warWeapon[2][i]) delete b->warWeapon[2][i];
		b = NULL;
		a = NULL;
		return tmp;
	}
	else if (tmp == 0) {
		if (a->cannotAct() && b->cannotAct()) {
			Warrior *c, *d;
			if (a->warName == "dragon")a->nYell = true;
			if (b->warName == "dragon")b->nYell = true;
			if (a->warHead->getHeadName() == redStr) {
				c = a; d = b;
			}
			else { 
				c = b; d = a; 
			}
			//000:40 both red iceman 1 and blue lion 12 were alive in city 2
			tmp = 1;
			cout << "both red " << c->warName << " " << c->warID << " and blue " << d->warName << " " << d->warID
				<< " were alive in city " << cityID << endl;
			actYell();
		}
		return tmp;
	}
}

//************************************************* HEADQUARTER *************************************************//  

Headquarter::Headquarter(int _n, int _ele, int _order[], int _h[], int _f[], int _k) :elements(_ele), headWarID(0) {
	orderIndex = 0;
	headName = _n == 0 ? "red" : "blue";
	headCanMake = true;
	headLionK = _k;
	for (int i = 0; i < 5; i++) {
		headOrder[i] = _order[i];
		headWarHealth[i] = _h[i];
		headWarForce[i] = _f[i];
	}
	if (headWarHealth[headOrder[orderIndex]] > elements) headCanMake = false;
}
/*Headquarter::~Headquarter() {
	for (int i = 0; i < headWarID; i++)
		delete headWarrior[i];
}*/

void Headquarter::actHeadMake(City *c) {
	if (!headCanMake) return;
	headWarID++;
	elements -= headWarHealth[headOrder[orderIndex]];
	//dragon ninja iceman lion wolf
	switch (headOrder[orderIndex])
	{
	case 0:
		headWarrior[headWarID] = new wDragon(this, c, headWarID, headWarHealth[0], headWarForce[0]);
		break;
	case 1:
		headWarrior[headWarID] = new wNinja(this, c, headWarID, headWarHealth[1], headWarForce[1]);
		break;
	case 2:
		headWarrior[headWarID] = new wIceman(this, c, headWarID, headWarHealth[2], headWarForce[2]);
		break;
	case 3:
		headWarrior[headWarID] = new wLion(this, c, headWarID, headWarHealth[3], headWarForce[3], headLionK);
		break;
	case 4:
		headWarrior[headWarID] = new wWolf(this, c, headWarID, headWarHealth[4], headWarForce[4]);
		break;
	default:
		break;
	}
	headWarrior[headWarID]->prtMake();
	orderIndex = (orderIndex + 1) % 5;
	if (headWarHealth[headOrder[orderIndex]] > elements) headCanMake = false;
}
void Headquarter::actWarGo() {
	for (int i = 1; i <= headWarID; i++) {
		if (headWarrior[i]->nRun) continue;
		headWarrior[i]->actGo();
	}
}
void Headquarter::prtEle() {
	coutTime();
	cout << "50 " << elements << " elements in " << headName << " headquarter" << endl;
}

bool Headquarter::actLionRun() {
	for (int i = 1; i <= headWarID; i++) {
		if (headWarrior[i]->nRun) continue;
		if (headWarrior[i]->warName != "lion") continue;
		return headWarrior[i]->actRun();
	}
	return false;
}

//************************************************* MAIN *************************************************//  

void controller() {
	gameend = false;
	timeID = -1;
	//iceman、lion、wolf、ninja、dragon 
	int orderred[5] = { 2,3,4,1,0 };
	Headquarter headRed(0, sum_elements, orderred, sum_health, sum_force, sum_k);
	//lion、dragon、ninja、iceman、wolf
	int orderblue[5] = { 3,0,1,2,4 };
	Headquarter headBlue(1, sum_elements, orderblue, sum_health, sum_force, sum_k);
	for (int i = 0; i <= sum_city + 1; i++) {
		cities[i] = new City(i);
	}
	while (1) {
		timeID++;
		
		//00
		headRed.actHeadMake(cities[0]);
		headBlue.actHeadMake(cities[sum_city + 1]);	
		sum_time -= 5;
		if (sum_time < 0) break;
		//05
		headRed.actLionRun();
		headBlue.actLionRun();
		sum_time -= 5;
		if (sum_time < 0) break;
		//10
		headRed.actWarGo();
		headBlue.actWarGo();

		for (int i = 0; i <= sum_city + 1; ++i) cities[i]->prtWar();
		if (gameend)break;

		sum_time -= 25;
		if (sum_time < 0) break;
		//35
		for (int i = 1; i <= sum_city; ++i) cities[i]->actWolfRob();
		sum_time -= 5;
		if (sum_time < 0) break;
		
		//40
		for (int i = 1; i <= sum_city; ++i) cities[i]->actGameCtrl();
		//for (int i = 1; i <= sum_city; ++i) cities[i]->actYell();
		sum_time -= 10;
		if (sum_time < 0) break;
		//50
		headRed.prtEle();
		headBlue.prtEle();
		sum_time -= 5;
		if (sum_time < 0) break;
		//55
		for (int i = 1; i <= sum_city; ++i) cities[i]->prtWeaponNum();
		sum_time -= 5;
		if (sum_time < 0) break;

		//cout << "sum_time: " << sum_time << endl;
	}
}

int main() {
	int n;
	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> sum_elements >> sum_city >> sum_k >> sum_time;
		for (int i = 0; i < 5; ++i)cin >> sum_health[i];
		for (int i = 0; i < 5; i++)cin >> sum_force[i];
		cout << "Case " << i << ":" << endl;
		controller();
	}
	return 0;
}
