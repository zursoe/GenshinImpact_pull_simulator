using namespace std;

bool isUP; // 用于标记上一次抽到的5星是常驻还是当期UP
int simulations = 10000; // 模拟次数，可以增加以获得更准确的结果

double generateRandom() {
    static random_device rd;
    static mt19937 generator(rd());
    static uniform_real_distribution<> distribution(0.0, 1.0);
    return distribution(generator);
}

int simulate(int tickets) {
    double probability = 0.006; // 初始概率
    int count = 0;
    int upCount = 0;
    bool lastUP = isUP;

    while (tickets--) {             //粉球--
        count++;                        //抽卡次数计数++
        double randNum = generateRandom();//随机的一个概率%数字，即真实抽卡情况

        if (randNum < probability) {     //真实抽卡小于概率，抽中5星了！！！
            if ((lastUP && generateRandom() < 0.5) || !lastUP) {       //&&是与，||是或，（上一次歪了）或者（上一次没歪，但是随机概率小于0.5即这次也没歪），那么这一次不会歪。
                upCount++;                  //抽中的up5星计数+1
                lastUP = true;              //这一次没歪，所以改为true
            }
            else {                         //歪了
                lastUP = false;            //歪了
            }
            count = 0;                //这个if是抽中5星了，所以计数器清零，下一抽是第一抽。
            probability = 0.006;      //下一抽是第一抽，第一抽概率是0.006
        }
        else if (count > 73) {        //没出金，且是73抽以后
            probability += 0.06;      //出金概率每次+0.06即每次＋6%
            if (probability > 1) probability = 1;         //概率累加大于1时，就是1，比如第90抽算下来概率是102.6%，就要改为1
        }
    }

    return upCount;
}

int wishcalculator() {
    srand(time(0));
    int tickets;

    cout << "你上一次歪了没？（歪了扣0，没歪扣1）" << "\n" << "Did u lose the 50 / 50 last time ? (lose:0, win : 1)" << "\n";
    cin >> isUP;
    if (isUP != 0 && isUP != 1) {
        cout << "输入错误，重启程序！" << "\n" << "Input error!" << "\n" << endl;
        system("pause"); // 在用户按任意键后，程序结束
        return 0;
    }

    cout << "\n" << "你现在多少粉球（纠缠之缘）？（最低10个，最高1260个，1纠缠之缘 = 160原石）" << "\n" << "Enter the number of Interteined fates you have(between 10 and 1260, 1 Intertwined fate = 160 primogems) :" << "\n";
    cin >> tickets;
    if (tickets < 10 || tickets > 1260) {
        cout << "输入错误，重启程序！" << "\n" << "Input error!" << "\n" << endl;
        system("pause"); // 在用户按任意键后，程序结束
        return 0;
    }

    cout << "\n" << "模拟结果如下：" << "\n" << "The simulation results are as follows:" << "\n" << endl;

    vector<int> results(20, 0); // 用于存储每次模拟的结果，创建一个名为 results 的整数向量，并初始化该向量的大小为 20 个元素，每个元素的初始值为 0。
    for (int i = 0; i < simulations; i++) {  //开始大量抽卡
        int upCount = simulate(tickets);
        results[upCount]++;    //举例解释：如果upCount为2（表示在模拟中抽到2个当期UP五星角色），则results[2]的值将增加1，表示模拟结果是抽到2个当期UP五星角色的情况次数+1。
    }

    cout << fixed << setprecision(3);    //设置浮点数的输出格式。fixed 使得浮点数以固定小数点的方式输出，setprecision(1) 设置小数点后的位数为1。
    for (int i = 0; i < 20; i++) {
        double allprobablity = 0;
        if (results[i] != 0) {
            for (int j = i; j < 20; j++) {
                allprobablity += static_cast<double>(results[j]) / simulations * 100;
                //cout << allprobablity << "+" << static_cast<double>(results[j]) / simulations * 100 << endl;    //测试时使用的
            }
            cout << "你至少抽出" << i << "个当期up的5星角色的概率是" << allprobablity << "%" << endl;
        }
    }
    cout << "\n" << endl;
    for (int i = 0; i < 20; i++) {
        double allprobablity = 0;
        if (results[i] != 0) {
            for (int j = i; j < 20; j++) {
                allprobablity += static_cast<double>(results[j]) / simulations * 100;
            }
            cout << "The probability that you get at least " << i << " up 5 star character(s) is " << allprobablity << "%" << endl;
        }
    }
    cout << "\n" << endl;
    for (int i = 0; i < 20; i++) {
        double allprobablity = 0;
        if (results[i] != 0) {
            cout << "你恰好（不多不少）抽出" << i << "个当期up的5星角色的概率是" << static_cast<double>(results[i]) / simulations * 100 << " % " << endl;
        }
    }
    cout << "\n" << endl;
    for (int i = 0; i < 20; i++) {
        double allprobablity = 0;
        if (results[i] != 0) {
            cout << "The probability that you get exactly (no more, no less) " << i << " up 5 star character(s) is" << static_cast<double>(results[i]) / simulations * 100 << "%" << endl;
        }
    }

    system("pause"); // 在用户按任意键后，程序结束
    return 0;
}
/*

抽卡机制:
1. 抽卡概率：
•	1-73抽：每次抽卡获得五星角色的概率是固定的0.6%。
•	74-90抽：从第74次开始，每次抽卡获得五星角色的概率在前一次的基础上增加6%。
•	90+抽：第90次抽卡及之后的抽卡获得五星角色的概率为100%。
2. 五星角色分类：
•	抽到五星角色后，有两种可能：当期up五星和常驻五星。
•	如果上一次抽到的是常驻五星，下一次一定会抽到当期up五星；如果上一次抽到的是当期up五星，下一次有50%的概率抽到常驻五星，50%的概率抽到当期up五星。
3. 抽卡次数重置：
•	每次抽到五星角色后，抽卡次数重置为0。

*/
