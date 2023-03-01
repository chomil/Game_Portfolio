using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MonsterStatus : MonoBehaviour
{

    public Text levelText;
    public Text expText;
    public Image expImage;
    public Text starText;
    public Text moonText;

    private GameObject curMonster = null;


    public void ShowMonsterStatus(GameObject monsterObject, bool isShow)
    {
        if(isShow==false)
        {
            if (curMonster!=null && monsterObject == curMonster)
            {
                gameObject.SetActive(false);
            }
            return;
        }

        gameObject.SetActive(true);
        curMonster = monsterObject;

        MonsterInfo monInfo = curMonster.GetComponent<Monster>().GetMonsterInfo();

        Vector3 pos = curMonster.transform.position;
        pos.y += 0.9f+ monInfo.level*0.2f;
        gameObject.transform.position = pos;

        levelText.text = string.Format("Lv.{0}  {1}", monInfo.level, GameManager.instance.monNameList[monInfo.id]);
        if(monInfo.level>=3)
        {
            expText.text = "MAX";
            expImage.rectTransform.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, 26);
        }
        else
        {
            expText.text = string.Format("{0} / {1}", monInfo.curExp, monInfo.maxExp);
            expImage.rectTransform.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, (float)monInfo.curExp / (float)monInfo.maxExp * 26);
        }
        int starLevel = GameManager.instance.GetSaveData().starSkillLevel;
        int monKind = GameManager.instance.MonsterKindNum();
        starText.text = string.Format("{0:n0}", monInfo.starPerClick * monInfo.level * Mathf.Min(starLevel, monKind));
        moonText.text = string.Format("{0:n0}", GameManager.instance.monSellPriceList[monInfo.id] * monInfo.level);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
