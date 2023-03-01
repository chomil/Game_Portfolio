using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Cloud : MonoBehaviour
{
    public float speed;

    private void Awake()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 transVec = new Vector3(Time.deltaTime * speed, 0, 0);
        transform.position += transVec;
        if(transform.position.x > 8)
        {
            transform.position += new Vector3(-16, 0, 0);
        }
        if (transform.position.x < -8)
        {
            transform.position += new Vector3(16, 0, 0);
        }
        float curX = transform.position.x;
        transform.position += new Vector3(0, Time.deltaTime * (curX / 220) * Mathf.Abs(speed)*10f, 0);
        transform.localEulerAngles = new Vector3(0, 0, (1 - Mathf.Cos((curX/8*90) * Mathf.PI / 180)) * ((curX<0)?30:-30));
    }
}
