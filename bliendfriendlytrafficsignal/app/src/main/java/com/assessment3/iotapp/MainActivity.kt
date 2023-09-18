package com.assessment3.iotapp

import android.annotation.SuppressLint
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.speech.tts.TextToSpeech
import android.util.Log
import android.view.Gravity
import android.widget.SeekBar
import android.widget.Switch
import android.widget.TextView
import androidx.appcompat.widget.SwitchCompat
import com.google.firebase.database.*
import com.google.firebase.database.ktx.getValue
import java.util.*

class MainActivity : AppCompatActivity(), TextToSpeech.OnInitListener {

   private  lateinit var redlight:SwitchCompat
   private lateinit var greenlight:SwitchCompat
   private lateinit var yellowlight:SwitchCompat
   private lateinit var databaseReference: DatabaseReference
   private lateinit var brightness:SeekBar
   private lateinit var ledstatus:Switch
   private lateinit var textspeech:TextToSpeech
   private lateinit var nearpoint:SwitchCompat
    private lateinit var nearpoint1:SwitchCompat
    private lateinit var time12:SwitchCompat
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        textspeech= TextToSpeech(this, this )
        redlight=findViewById(R.id.Redlight)
        greenlight=findViewById(R.id.Greenlight)
        yellowlight=findViewById(R.id.Yellowlight)
        nearpoint=findViewById(R.id.tcrt)
        nearpoint1=findViewById(R.id.tcrt1)
        time12=findViewById(R.id.time)
        databaseReference = FirebaseDatabase.getInstance().getReference()



        redlight.setOnCheckedChangeListener { _, isChecked ->
            val ledValue5 = if (isChecked) 1 else 0
            databaseReference.child("LED_RED").setValue(ledValue5)
        }
        yellowlight.setOnCheckedChangeListener { _, isChecked ->
            val ledValue6 = if (isChecked) 1 else 0
            databaseReference.child("LED_YELLOW").setValue(ledValue6)
        }
        greenlight.setOnCheckedChangeListener { _, isChecked ->
            val ledValue7 = if (isChecked) 1 else 0
            databaseReference.child("LED_GREEN").setValue(ledValue7)


        }
        time12.setOnCheckedChangeListener { _, isChecked ->
            val ledValue4 = if (isChecked) 1 else 0
            databaseReference.child("SOME_VALUE").setValue(ledValue4)


        }
        nearpoint.setOnCheckedChangeListener { _, isChecked ->
            val ledValue1 = if (isChecked) 1 else 0
            databaseReference.child("TCRT5000").setValue(ledValue1)

            // If the green LED is on, play a voice message
            if (ledValue1 == 1) {
                textspeech.speak("You are now at the traffic light point", TextToSpeech.QUEUE_FLUSH, null, null)
            }

        }
        nearpoint1.setOnCheckedChangeListener { _, isChecked ->
            val ledValue2 = if (isChecked) 1 else 0
            databaseReference.child("TCRT50001").setValue(ledValue2)

            // If the green LED is on, play a voice message
            if (ledValue2 == 1) {
                textspeech.speak("You are now at the traffic light point", TextToSpeech.QUEUE_FLUSH, null, null)
            }

        }
        databaseReference.addValueEventListener(object : ValueEventListener {

            override fun onDataChange(dataSnapshot: DataSnapshot) {
                // Get the humidity and temperature values from the dataSnapshot
                val humidityValue = dataSnapshot.child("Humidity/").getValue(Float::class.java)
                val temperatureValue = dataSnapshot.child("Temperature/").getValue(Float::class.java)
               // val switchstatus=dataSnapshot.child("relay").getValue(Int::class.java)
                val redlight1=dataSnapshot.child("LED_RED").getValue(Int::class.java)
                val greenlight2=dataSnapshot.child("LED_GREEN").getValue(Int::class.java)
                val yellowlight3=dataSnapshot.child("LED_YELLOW").getValue(Int::class.java)
                val tcrt=dataSnapshot.child("TCRT5000").getValue(Int::class.java)
                val tcrt1=dataSnapshot.child("TCRT50001").getValue(Int::class.java)
                val time=dataSnapshot.child("SOME_VALUE").getValue(Int::class.java)
                //val gasvalue=dataSnapshot.child("gas").getValue(Float::class.java)
                // Set the humidity and temperature values to the text views
               // humiditytext.text = "Humidity: \n$humidityValue"
               // humiditytext.gravity = Gravity.CENTER
               // temperaturetext.text = "Temperature: \n$temperatureValue"
               // temperaturetext.gravity = Gravity.CENTER
               // gas.text = "Gas Level: \n$gasvalue"
               // gas.gravity = Gravity.CENTER

                //ledstatus.isChecked = switchstatus == 1
                redlight.isChecked = redlight1 == 1
                greenlight.isChecked = greenlight2 == 1
                yellowlight.isChecked = yellowlight3 == 1
                nearpoint.isChecked=tcrt==1
                nearpoint1.isChecked=tcrt1==1
                time12.isChecked=time==1


                if ((nearpoint.isChecked || nearpoint1.isChecked) && greenlight.isChecked && time12.isChecked) {
                    textspeech.speak("You can cross the road now", TextToSpeech.QUEUE_FLUSH, null, null)
                }
            }

            override fun onCancelled(databaseError: DatabaseError) {
                // Handle database error
            }
        })
    }
    // Implement the onInit method of the TextToSpeech.OnInitListener interface
     override fun onInit(status: Int) {
        if (status == TextToSpeech.SUCCESS) {
            val result = textspeech.setLanguage(Locale.US)

            if (result == TextToSpeech.LANG_MISSING_DATA || result == TextToSpeech.LANG_NOT_SUPPORTED) {
                Log.e("TTS", "This language is not supported")
            }
        } else {
            Log.e("TTS", "Initialization failed")
        }
    }

    // Override the onDestroy method to release the TextToSpeech object
    override fun onDestroy() {
        super.onDestroy()

        textspeech.stop()
        textspeech.shutdown()
    }


}