using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MonsterShop : MonoBehaviour
{
    public Image monImageUnlock;
    public Text monNameUnlock;
    public Text monDetailUnlock;
    public Text monPriceUnlock;

    public Image monImageLock;
    public Text monPriceLock;
    public Text pageNum;
    public GameObject monUnlockGroup;
    public GameObject monLockGroup;

    private int monsterListNum;


    int shopPage = 0;

    public void MovePage(int move)
    {
        shopPage += move;
        shopPage = (shopPage < 0) ? 0 : shopPage;
        shopPage = (shopPage > monsterListNum - 1) ? monsterListNum - 1 : shopPage;

        SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Button);
        ChangePage();
    }

    public void UnlockPage()
    {
        if (GameManager.instance.GetSaveData().MonUnlockList[shopPage] == true)
        {
            return;
        }
        int unlockPrice = GameManager.instance.monUnlockPriceList[shopPage];
        if (unlockPrice <= GameManager.instance.GetSaveData().StarPoint)
        {
            GameManager.instance.GetSaveData().StarPoint -= unlockPrice;
            GameManager.instance.UnlockUpdate(shopPage, true);
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Unlock);
            ChangePage();

            GameManager.instance.GameSave();
        }
        else
        {
            //금액부족
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Fail);
            UIManager.instance.ShowRedMessage("별조각이 부족합니다");
        }
    }

    public void BuyMonster()
    {
        SaveData save = GameManager.instance.GetSaveData();
        if (save.MonUnlockList[shopPage] == false)
        {
            return;
        }
        if (save.upgradeNumLevel <= GameManager.instance.monObjectList.Count) //최대 몬스터 수 초과
        {
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Fail);
            UIManager.instance.ShowRedMessage("더 이상 몬스터를 데려올 수 없습니다");
            return;
        }
        int price = GameManager.instance.monSellPriceList[shopPage];

        if (price <= GameManager.instance.GetSaveData().MoonPoint)
        {
            GameManager.instance.GetSaveData().MoonPoint -= price;
            MonsterInfo monsterInfo = new MonsterInfo(shopPage, 1,0);
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Buy);
            GameManager.instance.CreateMonster(monsterInfo);
            GameManager.instance.GameSave();
        }
        else
        {
            //금액부족
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Fail);
            UIManager.instance.ShowRedMessage("달조각이 부족합니다");
        }
    }

    


    public void ChangePage()
    {
        while(GameManager.instance.GetSaveData().MonUnlockList.Count - 1 < shopPage)
        {
            GameManager.instance.GetSaveData().MonUnlockList.Add(false);
        }

        if (GameManager.instance.GetSaveData().MonUnlockList[shopPage] == true)
        {
            monUnlockGroup.SetActive(true);
            monLockGroup.SetActive(false);

            monImageUnlock.sprite = GameManager.instance.monSpriteList[shopPage];
            monImageUnlock.SetNativeSize();
            monNameUnlock.text = GameManager.instance.monNameList[shopPage];
            switch (shopPage)
            {
                case 0:
                    monDetailUnlock.text = "에버블루의 맑은 강물에\n서식하는 몬스터";
                    break;
                case 1:
                    monDetailUnlock.text = "뚫어져라 쳐다보는 뱀";
                    break;
                case 2:
                    monDetailUnlock.text = "떠돌이 토마토\n대체로 싱겁다";
                    break;
                case 3:
                    monDetailUnlock.text = "에버블루 항구 근처에서\n종종 목격되는 복어 몬스터";
                    break;
                case 4:
                    monDetailUnlock.text = "단단한 부리가\n무기의 재료가 되곤한다";
                    break;
                case 5:
                    monDetailUnlock.text = "뾰족한 껍질 속에\n맛있는 속살이 일품이다";
                    break;
                case 6:
                    monDetailUnlock.text = "파랑새의 알은\n비싸게 거래된다고한다";
                    break;
                case 7:
                    monDetailUnlock.text = "화살같은 가시가 있어서\n꺾으려는 여행자들을 공격한다";
                    break;
            }
            monPriceUnlock.text = string.Format("{0:n0}", GameManager.instance.monSellPriceList[shopPage]);
        }
        else
        {
            monUnlockGroup.SetActive(false);
            monLockGroup.SetActive(true);

            monImageLock.sprite = GameManager.instance.monSpriteList[shopPage];
            monImageLock.SetNativeSize();
            monPriceLock.text = string.Format("{0:n0}",GameManager.instance.monUnlockPriceList[shopPage]);
        }
        pageNum.text = string.Format("#{0:00}", (shopPage + 1));
    }

    // Start is called before the first frame update
    void Awake()
    {

    }
    void Start()
    {
        monsterListNum = GameManager.instance.monNameList.Length;
        ChangePage();
    }

    

    // Update is called once per frame
    void Update()
    {
        
    }
}
