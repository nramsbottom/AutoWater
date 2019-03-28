using System;

namespace PlantWatch
{
    internal class PortEvent
    {
        public int Type { get; set; }
        public string Message { get; set; }
        public DateTime ReceivedUtc { get; set; }
    }
}
