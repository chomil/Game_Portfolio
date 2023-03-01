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
            //�ݾ׺���
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Fail);
            UIManager.instance.ShowRedMessage("�������� �����մϴ�");
        }
    }

    public void BuyMonster()
    {
        SaveData save = GameManager.instance.GetSaveData();
        if (save.MonUnlockList[shopPage] == false)
        {
            return;
        }
        if (save.upgradeNumLevel <= GameManager.instance.monObjectList.Count) //�ִ� ���� �� �ʰ�
        {
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Fail);
            UIManager.instance.ShowRedMessage("�� �̻� ���͸� ������ �� �����ϴ�");
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
            //�ݾ׺���
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Fail);
            UIManager.instance.ShowRedMessage("�������� �����մϴ�");
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
                    monDetailUnlock.text = "��������� ���� ������\n�����ϴ� ����";
                    break;
                case 1:
                    monDetailUnlock.text = "�վ����� �Ĵٺ��� ��";
                    break;
                case 2:
                    monDetailUnlock.text = "������ �丶��\n��ü�� �̴̰�";
                    break;
                case 3:
                    monDetailUnlock.text = "������� �ױ� ��ó����\n���� ��ݵǴ� ���� ����";
                    break;
                case 4:
                    monDetailUnlock.text = "�ܴ��� �θ���\n������ ��ᰡ �ǰ��Ѵ�";
                    break;
                case 5:
                    monDetailUnlock.text = "������ ���� �ӿ�\n���ִ� �ӻ��� ��ǰ�̴�";
                    break;
                case 6:
                    monDetailUnlock.text = "�Ķ����� ����\n��ΰ� �ŷ��ȴٰ��Ѵ�";
                    break;
                case 7:
                    monDetailUnlock.text = "ȭ�찰�� ���ð� �־\n�������� �����ڵ��� �����Ѵ�";
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
