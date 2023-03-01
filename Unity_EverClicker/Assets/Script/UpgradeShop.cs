using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UpgradeShop : MonoBehaviour
{
    public Text upgradeNumText;
    public Text upgradeNumSubText;
    public Text upgradeNumPrice;

    public Text upgradeTimeText;
    public Text upgradeTimeSubText;
    public Text upgradeTimePrice;


    public void LevelUpgradeNum()
    {
        SaveData save = GameManager.instance.GetSaveData();
        if (save.upgradeNumLevel >= 20)
        {
            return;
        }
        int price = GameManager.instance.upgradeNumStarPriceList[save.upgradeNumLevel];
        if (price <= save.StarPoint)
        {
            save.upgradeNumLevel++;
            save.StarPoint -= price;
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Unlock);
            GameManager.instance.GameSave();
        }
        else
        {
            //�ݾ׺���
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Fail);
            UIManager.instance.ShowRedMessage("�������� �����մϴ�");
        }
        UpdatePage();
    }

    public void LevelUpgradeTime()
    {
        SaveData save = GameManager.instance.GetSaveData();
        if (save.upgradeTimeLevel>=5)
        {
            return;
        }
        int price = GameManager.instance.upgradeTimeMoonPriceList[save.upgradeTimeLevel];
        if (GameManager.instance.monObjectList.Count==0 && price + 100 > save.MoonPoint && save.MoonPoint >= price)
        {
            //����
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Fail);

            UIManager.instance.ShowRedMessage("�������� ��� ����ϸ� ������ ������ �� �����ϴ�");
            return;
        }
        if (price <= save.MoonPoint)
        {
            save.upgradeTimeLevel++;
            save.MoonPoint -= price;
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Unlock);
            GameManager.instance.GameSave();
        }
        else
        {
            //�ݾ׺���
            SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Fail);

            UIManager.instance.ShowRedMessage("�������� �����մϴ�");
        }
        UpdatePage();
    }
    public void UpdatePage()
    {
        SaveData save = GameManager.instance.GetSaveData();


        upgradeNumText.text = string.Format("���� ���� Lv.{0}", (save.upgradeNumLevel));
        if (save.upgradeNumLevel >= 20)
        {
            upgradeNumSubText.text = string.Format("�ִ� ���� �� : {0}����", (save.upgradeNumLevel));
            upgradeNumPrice.text = "MAX";
        }
        else
        {
            upgradeNumSubText.text = string.Format("�ִ� ���� �� : {0}����\n���� ���� : {1}����", (save.upgradeNumLevel), (save.upgradeNumLevel + 1));
            upgradeNumPrice.text = string.Format("{0:n0}", GameManager.instance.upgradeNumStarPriceList[save.upgradeNumLevel]);
        }


        upgradeTimeText.text = string.Format("���� ���� Lv.{0}", (save.upgradeTimeLevel));
        if (save.upgradeTimeLevel >= 5)
        {
            upgradeTimeSubText.text = string.Format("�ڵ� ���� : {0}�� ����", 10 - (save.upgradeTimeLevel - 1) * 2);
            upgradeTimePrice.text = "MAX";
        }
        else
        {
            upgradeTimeSubText.text = string.Format("�ڵ� ���� : {0}�� ����\n���� ���� : {1}�� ����", 10 - (save.upgradeTimeLevel - 1) * 2, 10 - (save.upgradeTimeLevel) * 2);
            upgradeTimePrice.text = string.Format("{0:n0}", GameManager.instance.upgradeTimeMoonPriceList[save.upgradeTimeLevel]);
        }

    }

    private void Start()
    {
        UpdatePage();
    }
    void Update()
    {
        
    }
}
