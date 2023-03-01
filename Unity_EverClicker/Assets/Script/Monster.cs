using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class MonsterInfo
{
    public int id;
    public int level;
    public int curExp;
    public int maxExp;
    public int starPerClick;

    public MonsterInfo(int id, int level, int curExp)
    {
        this.id = id;
        this.level = level;
        this.curExp = curExp;
        maxExp = (id + 1) * (id + 1) * (id + 1) * 10;
        starPerClick = (id + 1);
    }

    public string MonsterInfoToJson()
    {
        return JsonUtility.ToJson(this);
    }
}

public class Monster : MonoBehaviour
{
    MonsterInfo monsterInfo = new MonsterInfo(0,1,0);
    

    public void SetMonsterInfo(MonsterInfo monsterInfo)
    {
        this.monsterInfo = monsterInfo;
        GameManager.instance.ChangeAc(monAnim, monsterInfo.level);
        switch (monsterInfo.id)
        {
            case 0:
            case 1:
            case 3:
            case 5:
                shadow.transform.localPosition = new Vector3(0, 0.15f, 0);
                break;
            case 2:
                shadow.transform.localPosition = new Vector3(0, 0.1f, 0);
                break;
            case 4:
            case 6:
            case 7:
                shadow.transform.localPosition = new Vector3(0, 0.05f, 0);
                break;
        }
    }

    public MonsterInfo GetMonsterInfo()
    {
        return monsterInfo;
    }

    public GameObject shadow;

    private enum STATE
    {
        IDLE, WALK, TOUCH, GRAB, STATE_END
    }

    private STATE curState = STATE.STATE_END;
    private STATE nxtState = STATE.WALK;


    private Vector3 monSpeed = new Vector2();

    private SpriteRenderer monSprite;
    private Animator monAnim;

    private Vector3 borderTopLeft;
    private Vector3 borderBottomRight;
    public void SetPositionRandom()
    {
        Vector3 newPos = Vector3.zero;
        newPos.x = Random.Range(borderTopLeft.x, borderBottomRight.x);
        newPos.y = Random.Range(borderBottomRight.y, borderTopLeft.y);
        transform.position = newPos;
    }


    private float grabTime = 0;

    // Start is called before the first frame update
    void Awake()
    {
        monSprite = GetComponent<SpriteRenderer>();
        monAnim = GetComponent<Animator>();

        borderTopLeft = GameObject.Find("TopLeft").GetComponent<Transform>().position;
        borderBottomRight = GameObject.Find("BottomRight").GetComponent<Transform>().position;
    }

    private void Start()
    {
        GameManager.instance.ChangeAc(monAnim, monsterInfo.level);
    }

    // Update is called once per frame
    void Update()
    {
        //State Update
        switch (curState) 
        {
            case STATE.IDLE:
                Vector3 inPos = transform.position;
                if (transform.position.x < borderTopLeft.x)
                {
                    inPos.x = borderTopLeft.x;
                }
                if (transform.position.x > borderBottomRight.x)
                {
                    inPos.x = borderBottomRight.x;
                }
                if (transform.position.y < borderBottomRight.y)
                {
                    inPos.y = borderBottomRight.y;
                }
                if (transform.position.y > borderTopLeft.y)
                {
                    inPos.y = borderTopLeft.y;
                }
                transform.position = Vector3.Lerp(transform.position, inPos, Time.deltaTime*10);
                transform.position = new Vector3(transform.position.x, transform.position.y, transform.position.y);
                break;

            case STATE.WALK:
                Vector3 newPos = transform.position + Time.deltaTime * monSpeed;
                if (newPos.x < borderTopLeft.x)
                {
                    monSpeed.x = Mathf.Abs(monSpeed.x);
                }
                if (newPos.x > borderBottomRight.x)
                {
                    monSpeed.x = -Mathf.Abs(monSpeed.x);
                }
                if (newPos.y < borderBottomRight.y )
                {
                    monSpeed.y = Mathf.Abs(monSpeed.y);
                }
                if (newPos.y > borderTopLeft.y)
                {
                    monSpeed.y = -Mathf.Abs(monSpeed.y);
                }
                transform.Translate(Time.deltaTime * monSpeed);
                transform.position = new Vector3(transform.position.x, transform.position.y, transform.position.y);
                monSprite.flipX = (monSpeed.x < 0);
                break;

            case STATE.TOUCH:
                break;
            case STATE.GRAB:
                transform.position = GameManager.instance.GetMousePos();
                break;
        }

        ChangeState(nxtState);
    }

    private void OnMouseDown()
    {
        if(UIManager.instance.IsUIOpen())
        {
            return;
        }
        nxtState = STATE.TOUCH;
    }
    private void OnMouseDrag()
    {
        if (UIManager.instance.IsUIOpen())
        {
            return;
        }
        grabTime += Time.deltaTime;
        if(grabTime > 0.3f)
        {
            UIManager.instance.monsterStatusWindow.GetComponent<MonsterStatus>().ShowMonsterStatus(gameObject, true);
            nxtState = STATE.GRAB;
        }
    }

    
    private void OnMouseUp()
    {
        if (UIManager.instance.IsUIOpen())
        {
            return;
        }
        if (curState == STATE.GRAB)
        {
            UIManager.instance.monsterStatusWindow.GetComponent<MonsterStatus>().ShowMonsterStatus(gameObject, false);
            if (UIManager.instance.IsOnSell())
            {
                GameManager.instance.Sell(gameObject);
                SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Sell);
                Destroy(gameObject);
            }
        }
        grabTime = 0f;
        nxtState = STATE.IDLE;
    }

    private void ChangeState(STATE state)
    {
        if (curState == state)
        {
            return;
        }

        //State End
        switch (curState) 
        {
            case STATE.IDLE:
                StopCoroutine("MonsterIdleStart");
                break;

            case STATE.WALK:
                StopCoroutine("MonsterWalkStart");
                monAnim.SetBool("isWalk", false);
                break;

            case STATE.TOUCH:
                break;

            case STATE.GRAB:
                break;
        }

        //State Start
        switch (state) 
        {
            case STATE.IDLE:
                StartCoroutine("MonsterIdleStart");
                break;

            case STATE.WALK:
                StartCoroutine("MonsterWalkStart");
                break;

            case STATE.TOUCH:
                monAnim.SetTrigger("doTouch");
                SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Touch);
                int starLevel = GameManager.instance.GetSaveData().starSkillLevel;
                int monKind = GameManager.instance.MonsterKindNum();
                GameManager.instance.AddStarPoint(monsterInfo.starPerClick * monsterInfo.level * Mathf.Min(starLevel, monKind));
                if(monsterInfo.level < 3)
                {
                    int expLevel = GameManager.instance.GetSaveData().expSkillLevel;
                    monsterInfo.curExp += Mathf.Min(expLevel, monKind);
                    if (monsterInfo.curExp >= monsterInfo.maxExp)
                    {
                        SoundManager.instance.PlaySound(SoundManager.SFX_NAME.Grow);
                        monsterInfo.curExp = 0;
                        monsterInfo.level += 1;
                    }
                }
                GameManager.instance.ChangeAc(monAnim, monsterInfo.level);
                UIManager.instance.monsterStatusWindow.GetComponent<MonsterStatus>().ShowMonsterStatus(gameObject, true);
                nxtState = STATE.IDLE;
                break;

            case STATE.GRAB:
                break;
        }

        curState = state;
    }
    IEnumerator MonsterIdleStart()
    {
        float fTimeWait = Random.Range(2f, 3f);


        yield return new WaitForSeconds(fTimeWait);


        nxtState = STATE.WALK;
    }
    IEnumerator MonsterWalkStart()
    {
        UIManager.instance.monsterStatusWindow.GetComponent<MonsterStatus>().ShowMonsterStatus(gameObject, false);
        monAnim.SetBool("isWalk", true);
        float fTimeWalk = 1.5f + (Random.Range(0, 3)*0.5f);

        monSpeed.x = Random.Range(-1f, 1f);
        monSpeed.y = Random.Range(-1f, 1f);


        yield return new WaitForSeconds(fTimeWalk);
        monAnim.SetBool("isWalk", false);
        nxtState = STATE.IDLE;
    }
}